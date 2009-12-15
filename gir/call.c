/* Copyright (C) 2009 Free Software Foundation.
 *
 * Author: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *
 * This file is part of Guile-GIR.
 *
 * Guile-GIR is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Guile-GIR is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Guile-GIR.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "call.h"

/* SMOB types for Function call-related types */
scm_t_bits callable_info_t;
scm_t_bits g_i_function_info_t;
scm_t_bits callback_info_t;
scm_t_bits arg_info_t;

static SCM
gi_return_value_to_scm (GICallableInfo *info,
                        GArgument       return_value);
static SCM
gi_arg_to_scm (GITypeInfo *arg_type,
               GArgument   arg);
static void
scm_to_gi_arg (SCM         scm_arg,
               GITypeInfo *arg_type,
               GArgument  *arg);

static GArgument *
construct_in_args (GICallableInfo *callable_info,
                   SCM             scm_in_args,
                   int            *n_in_args);

static GArgument *
construct_out_args (GICallableInfo *callable_info,
                    int            *n_out_args,
                    int           **out_arg_indices);

static SCM
construct_return_value (GICallableInfo *callable_info,
                        GArgument       return_value,
                        GArgument      *out_args,
                        int             n_out_args,
                        int            *out_arg_indices);

static SCM
scm_g_function_info_invoke (SCM scm_info,
                            SCM scm_in_args)
{
        GIFunctionInfo *info;
        GICallableInfo *callable_info;
        int n_in_args;
        int n_out_args;
        int *out_arg_indices;
        int n_args;
        GArgument *in_args;
        GArgument *out_args;
        GArgument return_value;
        GError *error;

        info = (GIFunctionInfo *) SCM_SMOB_DATA (scm_info);
        callable_info = (GICallableInfo *) info;

        in_args = construct_in_args (callable_info, scm_in_args, &n_in_args);
        out_args = construct_out_args (callable_info,
                                       &n_out_args,
                                       &out_arg_indices);

        error = NULL;
        g_function_info_invoke (info,
                                in_args,
                                n_in_args,
                                out_args,
                                n_out_args,
                                &return_value,
                                &error);
        if (error) {
                /* FIXME: Throw Guile error here */
                g_critical ("Failed to load typelib: %s", error->message);
                return SCM_UNSPECIFIED;
        }

        return construct_return_value (callable_info,
                                       return_value,
                                       out_args,
                                       n_out_args,
                                       out_arg_indices);
}

static SCM
gi_return_value_to_scm (GICallableInfo *info,
                        GArgument       return_value)
{
        GITypeInfo *return_type;

        return_type = g_callable_info_get_return_type (info);

        return gi_arg_to_scm (return_type, return_value);
}

static SCM
gi_arg_to_scm (GITypeInfo *arg_type,
               GArgument   arg)
{
        switch (g_type_info_get_tag (arg_type)) {
                case GI_TYPE_TAG_UTF8:
                        return scm_from_locale_string (arg.v_string);
                default:
                        return SCM_UNSPECIFIED;
        }
}

static void
scm_to_gi_arg (SCM         scm_arg,
               GITypeInfo *arg_type,
               GArgument  *arg)
{
        switch (g_type_info_get_tag (arg_type)) {
                case GI_TYPE_TAG_UTF8:
                        arg->v_string = scm_to_locale_string (scm_arg);
                        break;
                default:
                        break;
        }
}

static GArgument *
construct_in_args (GICallableInfo *callable_info,
                   SCM             scm_in_args,
                   int            *n_in_args)
{
        GArgument *in_args;
        int n_args;
        int i;

        in_args = NULL;
        *n_in_args = 0;
        n_args = g_callable_info_get_n_args (callable_info);

        if (n_args < 0)
                return NULL;

        /* FIXME: We are allocating array for all arguments although it only
         *        needs to be big enough to fit 'in' arguments.
         */
        in_args = g_malloc0 (sizeof (GArgument) * n_args);

        for (i = 0; i < n_args; i++) {
                GIArgInfo *arg_info;
                GIDirection direction;
                GITypeInfo *type;
                SCM arg;

                arg_info = g_callable_info_get_arg (callable_info, i);
                direction = g_arg_info_get_direction (arg_info);
                if (direction != GI_DIRECTION_IN &&
                    direction != GI_DIRECTION_INOUT)
                        continue;

                type = g_arg_info_get_type (arg_info);

                arg = scm_list_ref (scm_in_args, scm_from_int (*n_in_args));
                scm_to_gi_arg (arg, type, &in_args[*n_in_args]);

                (*n_in_args)++;
        }

        return in_args;
}

static GArgument *
construct_out_args (GICallableInfo *callable_info,
                    int            *n_out_args,
                    int           **out_arg_indices)
{
        GArgument *out_args;
        int n_args;
        int i;

        out_args = NULL;
        *n_out_args = 0;
        n_args = g_callable_info_get_n_args (callable_info);

        if (n_args < 0)
                return NULL;

        /* FIXME: We are allocating array for all arguments although it only
         *        needs to be big enough to fit 'out' arguments.
         */
        out_args = g_malloc0 (sizeof (GArgument) * n_args);
        *out_arg_indices = g_malloc (sizeof (int) * n_args);

        for (i = 0; i < n_args; i++) {
                GIArgInfo *arg_info;
                GIDirection direction;

                arg_info = g_callable_info_get_arg (callable_info, i);
                direction = g_arg_info_get_direction (arg_info);
                if (direction != GI_DIRECTION_OUT &&
                    direction != GI_DIRECTION_INOUT)
                        continue;

                (*out_arg_indices)[*n_out_args] = i;

                (*n_out_args)++;
        }

        return out_args;
}

static SCM
construct_return_value (GICallableInfo *callable_info,
                        GArgument       return_value,
                        GArgument      *out_args,
                        int             n_out_args,
                        int            *out_arg_indices)
{
        SCM scm_return;
        SCM scm_return_value;

        scm_return = SCM_EOL;
        scm_return_value = gi_return_value_to_scm (callable_info, return_value);
        if (scm_return_value != SCM_UNSPECIFIED)
                scm_return = scm_cons (scm_return_value, scm_return);

        if (n_out_args > 0) {
                int i;

                for (i = 0; i < n_out_args; i++) {
                        GIArgInfo *arg_info;
                        GITypeInfo *type;
                        SCM arg;

                        arg_info = g_callable_info_get_arg (callable_info,
                                                            out_arg_indices[i]);
                        type = g_arg_info_get_type (arg_info);
                        arg = gi_arg_to_scm (type, out_args[i]);

                        if (scm_return_value != SCM_UNSPECIFIED)
                                scm_return = scm_cons (arg, scm_return);
                }
        }

        if (scm_return != SCM_EOL)
                return scm_values (scm_reverse (scm_return));
        else
                return SCM_UNSPECIFIED;
}

void
call_init ()
{
        callable_info_t = scm_make_smob_type ("g-i-callable-info", 0);

        g_i_function_info_t = scm_make_smob_type ("g-i-function-info", 0);
        scm_c_define_gsubr ("g-function-info-invoke",
                            2,
                            0,
                            0,
                            scm_g_function_info_invoke);

        callback_info_t = scm_make_smob_type ("g-i-callback-info", 0);
        arg_info_t = scm_make_smob_type ("g-i-arg-info", 0);
}
