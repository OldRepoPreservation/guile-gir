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

#include "registered-type.h"
#include <guile-gnome-gobject.h>

/* SMOB types for RegisteredTypeInfo */
scm_t_bits registered_type_info_t;

static SCM
scm_g_registered_type_info_get_g_type (SCM scm_info)
{
        GIRegisteredTypeInfo *info;
        GType gtype;

        info = (GIRegisteredTypeInfo *) SCM_SMOB_DATA (scm_info);
        gtype = g_registered_type_info_get_g_type (info);
        if (gtype == G_TYPE_INVALID)
                return SCM_BOOL_F;
        else {
                return scm_c_gtype_to_class (gtype);
        }
}

void
registered_type_init ()
{
        registered_type_info_t = scm_make_smob_type ("g-i-registered-type-info",
                                                     0);

        scm_c_define_gsubr ("g-registered-type-info-get-g-type",
                            1,
                            0,
                            0,
                            scm_g_registered_type_info_get_g_type);
}
