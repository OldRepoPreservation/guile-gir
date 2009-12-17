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

/* SMOB types for ObjectInfo */
scm_t_bits object_info_t;

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
}
