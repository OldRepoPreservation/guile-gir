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

#ifndef __TYPES_H__
#define __TYPES_H__

#include <glib.h>
#include <libguile.h>
#include <girepository.h>

extern scm_t_bits base_info_t;
extern scm_t_bits registered_type_info_t;
extern scm_t_bits struct_info_t;
extern scm_t_bits union_info_t;
extern scm_t_bits enum_info_t;
extern scm_t_bits object_info_t;
extern scm_t_bits interface_info_t;
extern scm_t_bits constant_info_t;
extern scm_t_bits value_info_t;
extern scm_t_bits signal_info_t;
extern scm_t_bits v_func_info_t;
extern scm_t_bits property_info_t;
extern scm_t_bits field_info_t;
extern scm_t_bits type_info_t;
extern scm_t_bits error_domain_info_t;
extern scm_t_bits unresolved_info_t;

G_GNUC_INTERNAL void
types_init                              (void);

#endif /* __TYPES_H__ */
