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

#include "callable.h"

/* SMOB types for CallableInfo & associated types */
scm_t_bits callable_info_t;
scm_t_bits callback_info_t;
scm_t_bits arg_info_t;

void
callable_init (void)
{
        callable_info_t = scm_make_smob_type ("g-i-callable-info", 0);
        callback_info_t = scm_make_smob_type ("g-i-callback-info", 0);
        arg_info_t = scm_make_smob_type ("g-i-arg-info", 0);
}
