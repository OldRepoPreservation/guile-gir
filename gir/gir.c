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

#include <glib.h>
#include <libguile.h>
#include <girepository.h>

/* SMOB types for GIRepository types */
static scm_t_bits repository_t;
static scm_t_bits base_info_t;
static scm_t_bits callable_info_t;
static scm_t_bits g_i_function_info_t;
static scm_t_bits callback_info_t;
static scm_t_bits registered_type_info_t;
static scm_t_bits struct_info_t;
static scm_t_bits union_info_t;
static scm_t_bits enum_info_t;
static scm_t_bits object_info_t;
static scm_t_bits interface_info_t;
static scm_t_bits constant_info_t;
static scm_t_bits value_info_t;
static scm_t_bits signal_info_t;
static scm_t_bits v_func_info_t;
static scm_t_bits property_info_t;
static scm_t_bits field_info_t;
static scm_t_bits arg_info_t;
static scm_t_bits type_info_t;
static scm_t_bits error_domain_info_t;
static scm_t_bits unresolved_info_t;
static scm_t_bits typelib_t;

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

static SCM
scm_g_irepository_get_default (void)
{
        SCM repository = scm_make_smob (repository_t);
        SCM_SET_SMOB_DATA (repository, g_irepository_get_default ());

        return repository;
}

/* FIXME: scm_flags is currently ignored */
static SCM
scm_g_irepository_require (SCM scm_repository,
                           SCM scm_namespace,
                           SCM scm_version,
                           SCM scm_flags)
{
        GIRepository *repo;
        GTypelib *typelib;
        GError *error;
        const char *version;
        SCM scm_typelib;
    
        repo = (GIRepository *) SCM_SMOB_DATA (scm_repository);

        if (SCM_UNBNDP (scm_version)) {
                version = NULL;
        } else {
                version = scm_to_locale_string (scm_version);
        }

        error = NULL;
        typelib = g_irepository_require (repo,
                                         scm_to_locale_string (scm_namespace),
                                         version,
                                         0,
                                         &error);
        if (error) {
                /* FIXME: Throw Guile error here */
                g_critical ("Failed to load typelib: %s", error->message);
                return SCM_UNSPECIFIED;
        }

        scm_typelib = scm_make_smob (typelib_t);
        SCM_SET_SMOB_DATA (scm_typelib, typelib);

        return scm_typelib;
}

static SCM
scm_g_irepository_find_by_name (SCM scm_repository,
                                SCM scm_namespace,
                                SCM scm_name)
{
        GIRepository *repo;
        GIBaseInfo *info;
        GError *error;
        SCM scm_info;

        repo = (GIRepository *) SCM_SMOB_DATA (scm_repository);

        error = NULL;
        info = g_irepository_find_by_name (repo,
                                           scm_to_locale_string (scm_namespace),
                                           scm_to_locale_string (scm_name));

        scm_info = scm_make_smob (base_info_t);
        SCM_SET_SMOB_DATA (scm_info, info);

        return scm_info;
}

static SCM
scm_g_function_info_invoke (SCM scm_info,
                            SCM scm_in_args,
                            SCM scm_out_args)
{
        GIFunctionInfo *info;
        int n_in_args;
        int n_out_args;
        GArgument *in_args;
        GArgument *out_args;
        GArgument return_value;
        SCM scm_return_value;
        GError *error;
    
        info = (GIFunctionInfo *) SCM_SMOB_DATA (scm_info);

        n_in_args = scm_ilength (scm_in_args);
        if (n_in_args > 0) {
                int i;

                in_args = g_malloc (sizeof (GArgument) * n_in_args);

                for (i = 0; i < n_in_args; i++) {
                        SCM arg;
                        GITypeInfo *type;

                        arg = scm_list_ref (scm_in_args, scm_from_int (i));
                        type = g_arg_info_get_type (g_callable_info_get_arg (
                                        (GICallableInfo *) info,
                                         i));

                        scm_to_gi_arg (arg, type, &in_args[i]);
                }
        } else {
                in_args = NULL;
        }

        n_out_args = scm_ilength (scm_out_args);
        if (n_out_args > 0) {
                out_args = g_malloc (sizeof (GArgument) * n_out_args);
        } else {
                out_args = NULL;
        }

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

        scm_return_value = gi_return_value_to_scm ((GICallableInfo *) info,
                                                   return_value);

        return scm_return_value;
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

void 
init ()
{
        g_type_init ();

        /* GIRepository */
        repository_t = scm_make_smob_type ("g-i-repository", 0);

        scm_c_define_gsubr ("g-irepository-get-default",
                            0,
                            0,
                            0,
                            scm_g_irepository_get_default);
        scm_c_define_gsubr ("g-irepository-require",
                            2,
                            2,
                            0,
                            scm_g_irepository_require);
        scm_c_define_gsubr ("g-irepository-find-by-name",
                            3,
                            0,
                            0,
                            scm_g_irepository_find_by_name);

        base_info_t = scm_make_smob_type ("g-i-base-info", 0);
        callable_info_t = scm_make_smob_type ("g-i-callable-info", 0);

        g_i_function_info_t = scm_make_smob_type ("g-i-function-info", 0);
        scm_c_define_gsubr ("g-function-info-invoke",
                            3,
                            0,
                            0,
                            scm_g_function_info_invoke);

        callback_info_t = scm_make_smob_type ("g-i-callback-info", 0);
        registered_type_info_t = scm_make_smob_type ("g-i-registered-type-info",
                                                     0);
        struct_info_t = scm_make_smob_type ("g-i-struct-info", 0);
        union_info_t = scm_make_smob_type ("g-i-union-info", 0);
        enum_info_t = scm_make_smob_type ("g-i-enum-info", 0);
        object_info_t = scm_make_smob_type ("g-i-object-info", 0);
        interface_info_t = scm_make_smob_type ("g-i-interface-info", 0);
        constant_info_t = scm_make_smob_type ("g-i-constant-info", 0);
        value_info_t = scm_make_smob_type ("g-i-value-info", 0);
        signal_info_t = scm_make_smob_type ("g-i-signal-info", 0);
        v_func_info_t = scm_make_smob_type ("g-i-v-func-info", 0);
        property_info_t = scm_make_smob_type ("g-i-property-info", 0);
        field_info_t = scm_make_smob_type ("g-i-field-info", 0);
        arg_info_t = scm_make_smob_type ("g-i-arg-info", 0);
        type_info_t = scm_make_smob_type ("g-i-type-info", 0);
        error_domain_info_t = scm_make_smob_type ("g-i-error-domain-info", 0);
        unresolved_info_t = scm_make_smob_type ("g-i-unresolved-info", 0);
        typelib_t = scm_make_smob_type ("g-typelib", 0);
}

