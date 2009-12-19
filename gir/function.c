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

#include <guile-gnome-gobject.h>
#include "function.h"
#include "callable.h"
#include "types.h"

/* Symbol names for GIFunctionInfoFlags */
#define FUNCTION_IS_METHOD      "g-i-function-is-method"
#define FUNCTION_IS_CONSTRUCTOR "g-i-function-is-constructor"
#define FUNCTION_IS_GETTER      "g-i-function-is-getter"
#define FUNCTION_IS_SETTER      "g-i-function-is-setter"
#define FUNCTION_WRAPS_VFUNC    "g-i-function-wraps-vfunc"
#define FUNCTION_THROWS         "g-i-function-throws"

/* SMOB type for GIFunctionInfo */
scm_t_bits function_info_t;

static SCM
gi_return_value_to_scm (GICallableInfo *info,
                        GArgument       return_value);
static SCM
gi_arg_to_scm (GITypeInfo *arg_type,
               GITransfer  transfer_type,
               GArgument   arg);
static void
scm_to_gi_arg (SCM         scm_arg,
               GITypeInfo *arg_type,
               GITransfer  transfer_type,
               GArgument  *arg);
static SCM
gi_interface_to_scm (GITypeInfo *arg_type,
                     GITransfer  transfer_type,
                     GArgument   arg);
static void
scm_to_gi_interface (SCM        scm_arg,
                     GIInfoType arg_type,
                     GITransfer transfer_type,
                     GArgument *arg);
static GArgument *
construct_in_args (GICallableInfo *callable_info,
                   SCM             scm_in_args,
                   int            *n_in_args);

static GArgument *
construct_out_args (GICallableInfo *callable_info,
                    GArgument     **real_out_args,
                    int            *n_out_args,
                    int           **out_arg_indices);

static SCM
construct_return_value (GICallableInfo *callable_info,
                        GArgument       return_value,
                        GArgument      *out_args,
                        int             n_out_args,
                        int            *out_arg_indices);

static SCM
gi_return_value_to_scm (GICallableInfo *info,
                        GArgument       return_value)
{
        GITypeInfo *return_type;
        GITransfer  transfer_type;

        return_type = g_callable_info_get_return_type (info);
        transfer_type = g_callable_info_get_caller_owns (info);

        return gi_arg_to_scm (return_type, transfer_type, return_value);
}

static SCM
gi_arg_to_scm (GITypeInfo *arg_type,
               GITransfer  transfer_type,
               GArgument   arg)
{
        switch (g_type_info_get_tag (arg_type)) {
                case GI_TYPE_TAG_BOOLEAN:
                        return scm_from_bool (arg.v_boolean);
                case GI_TYPE_TAG_INT8:
                        return scm_from_int8 (arg.v_int8);
                case GI_TYPE_TAG_UINT8:
                        return scm_from_uint8 (arg.v_uint8);
                case GI_TYPE_TAG_INT16:
                        return scm_from_int16 (arg.v_int16);
                case GI_TYPE_TAG_UINT16:
                        return scm_from_uint16 (arg.v_uint16);
                case GI_TYPE_TAG_INT32:
                        return scm_from_int32 (arg.v_int32);
                case GI_TYPE_TAG_UINT32:
                        return scm_from_uint32 (arg.v_uint32);
                case GI_TYPE_TAG_INT64:
                        return scm_from_int64 (arg.v_int64);
                case GI_TYPE_TAG_UINT64:
                        return scm_from_uint64 (arg.v_uint64);
                case GI_TYPE_TAG_INT:
                        return scm_from_int (arg.v_int);
                case GI_TYPE_TAG_UINT:
                        return scm_from_uint (arg.v_uint);
                case GI_TYPE_TAG_LONG:
                case GI_TYPE_TAG_TIME_T:
                        return scm_from_long (arg.v_long);
                case GI_TYPE_TAG_ULONG:
                        return scm_from_ulong (arg.v_ulong);
                case GI_TYPE_TAG_SSIZE:
                        return scm_from_ssize_t (arg.v_ssize);
                case GI_TYPE_TAG_SIZE:
                case GI_TYPE_TAG_GTYPE:
                        return scm_from_size_t (arg.v_size);
                case GI_TYPE_TAG_FLOAT:
                        return scm_from_double (arg.v_float);
                case GI_TYPE_TAG_DOUBLE:
                        return scm_from_double (arg.v_double);
                case GI_TYPE_TAG_FILENAME:
                case GI_TYPE_TAG_UTF8:
                        return scm_from_locale_string (arg.v_string);
                case GI_TYPE_TAG_INTERFACE:
                        return gi_interface_to_scm (arg_type,
                                                    transfer_type,
                                                    arg);
                default:
                        return SCM_UNSPECIFIED;
        }
}

static SCM
gi_interface_to_scm (GITypeInfo *arg_type,
                     GITransfer  transfer_type,
                     GArgument   arg)
{
        GIBaseInfo *base_info;
        gpointer c_instance;
        SCM scm_instance;

        base_info = g_type_info_get_interface (arg_type);
        c_instance = arg.v_pointer;

        switch (g_base_info_get_type (base_info)) {
                case GI_INFO_TYPE_OBJECT:
                case GI_INFO_TYPE_INTERFACE:
                        scm_instance = scm_c_gtype_instance_to_scm (c_instance);
                        if (transfer_type == GI_TRANSFER_EVERYTHING)
                                scm_c_gtype_instance_unref (c_instance);

                        break;
                case GI_INFO_TYPE_BOXED:
                {
                        GIRegisteredTypeInfo *reg_type;
                        GType gtype;

                        reg_type = (GIRegisteredTypeInfo *) arg_type;
                        gtype = g_registered_type_info_get_g_type (reg_type);

                        if (transfer_type == GI_TRANSFER_EVERYTHING)
                                scm_instance = scm_c_gvalue_new_take_boxed
                                                                (gtype,
                                                                 c_instance);
                        else
                                scm_instance = scm_c_gvalue_new_from_boxed
                                                                (gtype,
                                                                 c_instance);

                        break;
                }
                default:
                        scm_instance = SCM_UNSPECIFIED;
        }

        return scm_instance;
}

static void
scm_to_gi_arg (SCM         scm_arg,
               GITypeInfo *arg_type,
               GITransfer  transfer_type,
               GArgument  *arg)
{
        switch (g_type_info_get_tag (arg_type)) {
                case GI_TYPE_TAG_BOOLEAN:
                        arg->v_boolean = scm_to_bool (scm_arg);
                        break;
                case GI_TYPE_TAG_INT8:
                        arg->v_int8 = scm_to_int8 (scm_arg);
                        break;
                case GI_TYPE_TAG_UINT8:
                        arg->v_uint8 = scm_to_uint8 (scm_arg);
                        break;
                case GI_TYPE_TAG_INT16:
                        arg->v_int16 = scm_to_int16 (scm_arg);
                        break;
                case GI_TYPE_TAG_UINT16:
                        arg->v_uint16 = scm_to_uint16 (scm_arg);
                        break;
                case GI_TYPE_TAG_INT32:
                        arg->v_int = scm_to_int32 (scm_arg);
                        break;
                case GI_TYPE_TAG_UINT32:
                        arg->v_int = scm_to_int32 (scm_arg);
                        break;
                case GI_TYPE_TAG_INT64:
                        arg->v_int64 = scm_to_int64 (scm_arg);
                        break;
                case GI_TYPE_TAG_UINT64:
                        arg->v_uint64 = scm_to_uint64 (scm_arg);
                        break;
                case GI_TYPE_TAG_INT:
                        arg->v_int = scm_to_int (scm_arg);
                        break;
                case GI_TYPE_TAG_UINT:
                        arg->v_uint = scm_to_uint (scm_arg);
                        break;
                case GI_TYPE_TAG_TIME_T:
                case GI_TYPE_TAG_LONG:
                        arg->v_long = scm_to_long (scm_arg);
                        break;
                case GI_TYPE_TAG_ULONG:
                        arg->v_ulong = scm_to_ulong (scm_arg);
                        break;
                case GI_TYPE_TAG_SSIZE:
                        arg->v_size = scm_to_size_t (scm_arg);
                        break;
                case GI_TYPE_TAG_SIZE:
                case GI_TYPE_TAG_GTYPE:
                        arg->v_ssize = scm_to_ssize_t (scm_arg);
                        break;
                case GI_TYPE_TAG_FLOAT:
                        arg->v_float = scm_to_double (scm_arg);
                        break;
                case GI_TYPE_TAG_DOUBLE:
                        arg->v_double = scm_to_double (scm_arg);
                        break;
                case GI_TYPE_TAG_UTF8:
                case GI_TYPE_TAG_FILENAME:
                        arg->v_string = scm_to_locale_string (scm_arg);
                        break;
                case GI_TYPE_TAG_INTERFACE:
                {
                        GIBaseInfo *base_info;

                        base_info = g_type_info_get_interface (arg_type);

                        scm_to_gi_interface (scm_arg,
                                             g_base_info_get_type (base_info),
                                             transfer_type,
                                             arg);
                        break;
                }
                default:
                        break;
        }
}

static void
scm_to_gi_interface (SCM        scm_arg,
                     GIInfoType arg_type,
                     GITransfer transfer_type,
                     GArgument *arg)
{
        gpointer *c_instance;
        SCM scm_instance;

        c_instance = &(arg->v_pointer);

        switch (arg_type) {
                case GI_INFO_TYPE_OBJECT:
                case GI_INFO_TYPE_INTERFACE:
                        if (transfer_type == GI_TRANSFER_EVERYTHING)
                                scm_c_gtype_instance_ref (*c_instance);

                        *c_instance = scm_c_scm_to_gtype_instance (scm_arg);

                        break;
                case GI_INFO_TYPE_BOXED:
                {
                        GIRegisteredTypeInfo *reg_type;
                        GType gtype;

                        reg_type = (GIRegisteredTypeInfo *) arg_type;
                        gtype = g_registered_type_info_get_g_type (reg_type);

                        if (transfer_type == GI_TRANSFER_EVERYTHING)
                                *c_instance = scm_c_gvalue_dup_boxed (scm_arg);
                        else
                                *c_instance = scm_c_gvalue_peek_boxed (scm_arg);

                        break;
                }
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

        /* FIXME: We are allocating array for all arguments although it only
         *        needs to be big enough to fit 'in' arguments.
         */
        if ((g_function_info_get_flags ((GIFunctionInfo *) callable_info) &
             GI_FUNCTION_IS_METHOD) != 0) {
                GIBaseInfo *container;
                GIInfoType type;
                SCM arg;

                in_args = g_malloc0 (sizeof (GArgument) * (n_args + 1));

                container = g_base_info_get_container (
                                        (GIBaseInfo *) callable_info);
                type = g_base_info_get_type (container);

                arg = scm_list_ref (scm_in_args, scm_from_int (*n_in_args));
                scm_to_gi_interface (arg,
                                     type,
                                     GI_TRANSFER_NOTHING,
                                     &in_args[*n_in_args]);

                (*n_in_args)++;
        } else
                in_args = g_malloc0 (sizeof (GArgument) * n_args);

        for (i = 0; i < n_args; i++) {
                GIArgInfo *arg_info;
                GIDirection direction;
                GITransfer transfer_type;
                GITypeInfo *type;
                SCM arg;

                arg_info = g_callable_info_get_arg (callable_info, i);
                direction = g_arg_info_get_direction (arg_info);
                if (direction != GI_DIRECTION_IN &&
                    direction != GI_DIRECTION_INOUT)
                        continue;

                type = g_arg_info_get_type (arg_info);
                transfer_type = g_arg_info_get_ownership_transfer (arg_info);

                arg = scm_list_ref (scm_in_args, scm_from_int (*n_in_args));
                scm_to_gi_arg (arg, type, transfer_type, &in_args[*n_in_args]);

                (*n_in_args)++;
        }

        return in_args;
}

static GArgument *
construct_out_args (GICallableInfo *callable_info,
                    GArgument     **real_out_args,
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
        *real_out_args = g_malloc0 (sizeof (GArgument) * n_args);
        *out_arg_indices = g_malloc (sizeof (int) * n_args);

        for (i = 0; i < n_args; i++) {
                GIArgInfo *arg_info;
                GIDirection direction;

                arg_info = g_callable_info_get_arg (callable_info, i);
                direction = g_arg_info_get_direction (arg_info);
                if (direction != GI_DIRECTION_OUT &&
                    direction != GI_DIRECTION_INOUT)
                        continue;

                out_args[*n_out_args].v_pointer =
                                        &(*real_out_args)[*n_out_args];
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
                        GITransfer transfer_type;
                        GArgument *argument;
                        SCM scm_arg;

                        arg_info = g_callable_info_get_arg (callable_info,
                                                            out_arg_indices[i]);
                        type = g_arg_info_get_type (arg_info);
                        transfer_type = g_arg_info_get_ownership_transfer
                                                                (arg_info);
                        argument = (GArgument *) out_args[i].v_pointer;
                        scm_arg = gi_arg_to_scm (type,
                                                 transfer_type,
                                                 *argument);

                        if (scm_arg != SCM_UNSPECIFIED)
                                scm_return = scm_cons (scm_arg, scm_return);
                }
        }

        if (scm_return != SCM_EOL)
                return scm_values (scm_reverse (scm_return));
        else
                return SCM_UNSPECIFIED;
}

static SCM
scm_g_function_info_get_symbol (SCM scm_function_info)
{
        GIFunctionInfo *function_info;
        const gchar *symbol;

        function_info = (GIFunctionInfo *) SCM_SMOB_DATA (scm_function_info);
        symbol = g_function_info_get_symbol (function_info);
        if (symbol == NULL)
                return SCM_BOOL_F;
        else {
                return scm_from_locale_string (symbol);
        }
}

static SCM
scm_g_function_info_get_flags (SCM scm_function_info)
{
        GIFunctionInfo *function_info;

        function_info = (GIFunctionInfo *) SCM_SMOB_DATA (scm_function_info);

        return scm_from_int (g_function_info_get_flags (function_info));
}

static SCM
scm_g_function_info_get_property (SCM scm_function_info)
{
        GIFunctionInfo *function_info;
        GIPropertyInfo *property_info;
        SCM scm_return;

        function_info = (GIFunctionInfo *) SCM_SMOB_DATA (scm_function_info);
        property_info = g_function_info_get_property (function_info);
        if (property_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (property_info_t);
                SCM_SET_SMOB_DATA (scm_return, property_info);
        }

        return scm_return;
}

static SCM
scm_g_function_info_get_vfunc (SCM scm_function_info)
{
        GIFunctionInfo *function_info;
        GIVFuncInfo *vfunc_info;
        SCM scm_return;

        function_info = (GIFunctionInfo *) SCM_SMOB_DATA (scm_function_info);
        vfunc_info = g_function_info_get_vfunc (function_info);
        if (vfunc_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (v_func_info_t);
                SCM_SET_SMOB_DATA (scm_return, vfunc_info);
        }

        return scm_return;
}

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
        GArgument *real_out_args;
        GArgument return_value;
        GError *error;
        SCM scm_return;

        info = (GIFunctionInfo *) SCM_SMOB_DATA (scm_info);
        callable_info = (GICallableInfo *) info;

        in_args = construct_in_args (callable_info, scm_in_args, &n_in_args);
        out_args = construct_out_args (callable_info,
                                       &real_out_args,
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

        scm_return = construct_return_value (callable_info,
                                             return_value,
                                             out_args,
                                             n_out_args,
                                             out_arg_indices);

        g_free (in_args);
        g_free (out_args);
        g_free (out_arg_indices);
        g_free (real_out_args);

        return scm_return;
}

void
function_init ()
{
        function_info_t = scm_make_smob_type ("g-i-function-info", 0);
        scm_c_define_gsubr ("g-function-info-get-symbol",
                            1,
                            0,
                            0,
                            scm_g_function_info_get_symbol);
        scm_c_define_gsubr ("g-function-info-get-flags",
                            1,
                            0,
                            0,
                            scm_g_function_info_get_flags);
        scm_c_define_gsubr ("g-function-info-get-property",
                            1,
                            0,
                            0,
                            scm_g_function_info_get_property);
        scm_c_define_gsubr ("g-function-info-get-vfunc",
                            1,
                            0,
                            0,
                            scm_g_function_info_get_vfunc);
        scm_c_define_gsubr ("g-function-info-invoke",
                            2,
                            0,
                            0,
                            scm_g_function_info_invoke);

        scm_c_define (FUNCTION_IS_METHOD, scm_from_int (GI_FUNCTION_IS_METHOD));
        scm_c_define (FUNCTION_IS_CONSTRUCTOR,
                      scm_from_int (GI_FUNCTION_IS_CONSTRUCTOR));
        scm_c_define (FUNCTION_IS_GETTER, scm_from_int (GI_FUNCTION_IS_GETTER));
        scm_c_define (FUNCTION_IS_SETTER, scm_from_int (GI_FUNCTION_IS_SETTER));
        scm_c_define (FUNCTION_WRAPS_VFUNC,
                      scm_from_int (GI_FUNCTION_WRAPS_VFUNC));
        scm_c_define (FUNCTION_THROWS, scm_from_int (GI_FUNCTION_THROWS));
}
