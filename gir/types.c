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

#include "types.h"

/* SMOB types for GIRepository types */
scm_t_bits base_info_t;
scm_t_bits registered_type_info_t;
scm_t_bits struct_info_t;
scm_t_bits union_info_t;
scm_t_bits enum_info_t;
scm_t_bits interface_info_t;
scm_t_bits constant_info_t;
scm_t_bits value_info_t;
scm_t_bits signal_info_t;
scm_t_bits v_func_info_t;
scm_t_bits property_info_t;
scm_t_bits field_info_t;
scm_t_bits type_info_t;
scm_t_bits error_domain_info_t;
scm_t_bits unresolved_info_t;

void
types_init (void)
{
        base_info_t = scm_make_smob_type ("g-i-base-info", 0);
        registered_type_info_t = scm_make_smob_type ("g-i-registered-type-info",
                                                     0);
        struct_info_t = scm_make_smob_type ("g-i-struct-info", 0);
        union_info_t = scm_make_smob_type ("g-i-union-info", 0);
        enum_info_t = scm_make_smob_type ("g-i-enum-info", 0);
        interface_info_t = scm_make_smob_type ("g-i-interface-info", 0);
        constant_info_t = scm_make_smob_type ("g-i-constant-info", 0);
        value_info_t = scm_make_smob_type ("g-i-value-info", 0);
        signal_info_t = scm_make_smob_type ("g-i-signal-info", 0);
        v_func_info_t = scm_make_smob_type ("g-i-v-func-info", 0);
        property_info_t = scm_make_smob_type ("g-i-property-info", 0);
        field_info_t = scm_make_smob_type ("g-i-field-info", 0);
        type_info_t = scm_make_smob_type ("g-i-type-info", 0);
        error_domain_info_t = scm_make_smob_type ("g-i-error-domain-info", 0);
        unresolved_info_t = scm_make_smob_type ("g-i-unresolved-info", 0);
}
