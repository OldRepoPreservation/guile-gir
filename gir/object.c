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

#include "object.h"
#include "types.h"
#include "function.h"

/* SMOB types for ObjectInfo */
scm_t_bits object_info_t;

static SCM
scm_g_object_info_get_type_name (SCM scm_object_info)
{
        GIObjectInfo *object_info;
        const char *type_name;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        type_name = g_object_info_get_type_name (object_info);
        if (type_name == NULL)
                return SCM_BOOL_F;
        else
                return scm_from_locale_string (type_name);
}

static SCM
scm_g_object_info_get_type_init (SCM scm_object_info)
{
        GIObjectInfo *object_info;
        const char *type_init;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        type_init = g_object_info_get_type_init (object_info);
        if (type_init == NULL)
                return SCM_BOOL_F;
        else
                return scm_from_locale_string (type_init);
}

static SCM
scm_g_object_info_get_abstract (SCM scm_object_info)
{
        GIObjectInfo *object_info;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);

        return scm_from_bool (g_object_info_get_abstract (object_info));
}

static SCM
scm_g_object_info_get_parent (SCM scm_object_info)
{
        GIObjectInfo *object_info;
        GIObjectInfo *parent_info;
        SCM scm_return;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        parent_info = g_object_info_get_parent (object_info);
        if (parent_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (object_info_t);
                SCM_SET_SMOB_DATA (scm_return, parent_info);
        }

        return scm_return;
}

static SCM
scm_g_object_info_get_n_interfaces (SCM scm_object_info)
{
        GIObjectInfo *object_info;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);

        return scm_from_int (g_object_info_get_n_interfaces (object_info));
}

static SCM
scm_g_object_info_get_interface (SCM scm_object_info, SCM scm_n)
{
        GIObjectInfo *object_info;
        GIInterfaceInfo *interface_info;
        SCM scm_return;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        interface_info = g_object_info_get_interface (object_info,
                                                      scm_to_int (scm_n));
        if (interface_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (interface_info_t);
                SCM_SET_SMOB_DATA (scm_return, interface_info);
        }

        return scm_return;
}

static SCM
scm_g_object_info_get_n_fields (SCM scm_object_info)
{
        GIObjectInfo *object_info;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);

        return scm_from_int (g_object_info_get_n_fields (object_info));
}

static SCM
scm_g_object_info_get_field (SCM scm_object_info, SCM scm_n)
{
        GIObjectInfo *object_info;
        GIFieldInfo *field_info;
        SCM scm_return;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        field_info = g_object_info_get_field (object_info, scm_to_int (scm_n));
        if (field_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (field_info_t);
                SCM_SET_SMOB_DATA (scm_return, field_info);
        }

        return scm_return;
}

static SCM
scm_g_object_info_get_n_properties (SCM scm_object_info)
{
        GIObjectInfo *object_info;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);

        return scm_from_int (g_object_info_get_n_properties (object_info));
}

static SCM
scm_g_object_info_get_property (SCM scm_object_info, SCM scm_n)
{
        GIObjectInfo *object_info;
        GIPropertyInfo *property_info;
        SCM scm_return;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        property_info = g_object_info_get_property (object_info,
                                                    scm_to_int (scm_n));
        if (property_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (property_info_t);
                SCM_SET_SMOB_DATA (scm_return, property_info);
        }

        return scm_return;
}

static SCM
scm_g_object_info_get_n_methods (SCM scm_object_info)
{
        GIObjectInfo *object_info;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);

        return scm_from_int (g_object_info_get_n_methods (object_info));
}

static SCM
scm_g_object_info_get_method (SCM scm_object_info, SCM scm_n)
{
        GIObjectInfo *object_info;
        GIFunctionInfo *func_info;
        SCM scm_return;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        func_info = g_object_info_get_method (object_info, scm_to_int (scm_n));
        if (func_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (function_info_t);
                SCM_SET_SMOB_DATA (scm_return, func_info);
        }

        return scm_return;
}

static SCM
scm_g_object_info_find_method (SCM scm_object_info, SCM scm_name)
{
        GIObjectInfo *object_info;
        GIFunctionInfo *func_info;
        SCM scm_return;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        func_info = g_object_info_find_method (object_info,
                                               scm_to_locale_string (scm_name));
        if (func_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (function_info_t);
                SCM_SET_SMOB_DATA (scm_return, func_info);
        }

        return scm_return;
}


static SCM
scm_g_object_info_get_n_signals (SCM scm_object_info)
{
        GIObjectInfo *object_info;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);

        return scm_from_int (g_object_info_get_n_signals (object_info));
}

static SCM
scm_g_object_info_get_signal (SCM scm_object_info, SCM scm_n)
{
        GIObjectInfo *object_info;
        GISignalInfo *signal_info;
        SCM scm_return;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        signal_info = g_object_info_get_signal (object_info,
                                                scm_to_int (scm_n));
        if (signal_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (signal_info_t);
                SCM_SET_SMOB_DATA (scm_return, signal_info);
        }

        return scm_return;
}

static SCM
scm_g_object_info_get_n_vfuncs (SCM scm_object_info)
{
        GIObjectInfo *object_info;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);

        return scm_from_int (g_object_info_get_n_vfuncs (object_info));
}

static SCM
scm_g_object_info_get_vfunc (SCM scm_object_info, SCM scm_n)
{
        GIObjectInfo *object_info;
        GIVFuncInfo *vfunc_info;
        SCM scm_return;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        vfunc_info = g_object_info_get_vfunc (object_info, scm_to_int (scm_n));
        if (vfunc_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (v_func_info_t);
                SCM_SET_SMOB_DATA (scm_return, vfunc_info);
        }

        return scm_return;
}

static SCM
scm_g_object_info_get_n_constants (SCM scm_object_info)
{
        GIObjectInfo *object_info;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);

        return scm_from_int (g_object_info_get_n_constants (object_info));
}

static SCM
scm_g_object_info_get_constant (SCM scm_object_info, SCM scm_n)
{
        GIObjectInfo *object_info;
        GIConstantInfo *constant_info;
        SCM scm_return;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        constant_info = g_object_info_get_constant (object_info,
                                                    scm_to_int (scm_n));
        if (constant_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (constant_info_t);
                SCM_SET_SMOB_DATA (scm_return, constant_info);
        }

        return scm_return;
}

static SCM
scm_g_object_info_get_class_struct (SCM scm_object_info)
{
        GIObjectInfo *object_info;
        GIStructInfo *struct_info;
        SCM scm_return;

        object_info = (GIObjectInfo *) SCM_SMOB_DATA (scm_object_info);
        struct_info = g_object_info_get_class_struct (object_info);
        if (struct_info == NULL)
                return SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (struct_info_t);
                SCM_SET_SMOB_DATA (scm_return, struct_info);
        }

        return scm_return;
}

void
object_init ()
{
        object_info_t = scm_make_smob_type ("g-i-object-info", 0);

        scm_c_define_gsubr ("g-object-info-get-class-struct",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_class_struct);

        scm_c_define_gsubr ("g-object-info-get-type-name",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_type_name);
        scm_c_define_gsubr ("g-object-info-get-type-init",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_type_init);
        scm_c_define_gsubr ("g-object-info-get-abstract",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_abstract);
        scm_c_define_gsubr ("g-object-info-get-parent",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_parent);
        scm_c_define_gsubr ("g-object-info-get-n-interfaces",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_n_interfaces);
        scm_c_define_gsubr ("g-object-info-get-interface",
                            2,
                            0,
                            0,
                            scm_g_object_info_get_interface);
        scm_c_define_gsubr ("g-object-info-get-n-fields",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_n_fields);
        scm_c_define_gsubr ("g-object-info-get-field",
                            2,
                            0,
                            0,
                            scm_g_object_info_get_field);
        scm_c_define_gsubr ("g-object-info-get-n-properties",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_n_properties);
        scm_c_define_gsubr ("g-object-info-get-property",
                            2,
                            0,
                            0,
                            scm_g_object_info_get_property);
        scm_c_define_gsubr ("g-object-info-get-n-methods",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_n_methods);
        scm_c_define_gsubr ("g-object-info-get-method",
                            2,
                            0,
                            0,
                            scm_g_object_info_get_method);
        scm_c_define_gsubr ("g-object-info-find-method",
                            2,
                            0,
                            0,
                            scm_g_object_info_find_method);
        scm_c_define_gsubr ("g-object-info-get-n-signals",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_n_signals);
        scm_c_define_gsubr ("g-object-info-get-signal",
                            2,
                            0,
                            0,
                            scm_g_object_info_get_signal);
        scm_c_define_gsubr ("g-object-info-get-n-vfuncs",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_n_vfuncs);
        scm_c_define_gsubr ("g-object-info-get-vfunc",
                            2,
                            0,
                            0,
                            scm_g_object_info_get_vfunc);
        scm_c_define_gsubr ("g-object-info-get-n-constants",
                            1,
                            0,
                            0,
                            scm_g_object_info_get_n_constants);
        scm_c_define_gsubr ("g-object-info-get-constant",
                            2,
                            0,
                            0,
                            scm_g_object_info_get_constant);
}
