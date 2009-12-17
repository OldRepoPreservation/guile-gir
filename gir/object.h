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

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <glib.h>
#include <libguile.h>
#include <girepository.h>

/* SMOB types for ObjectInfo */
extern scm_t_bits object_info_t;

static SCM
scm_g_object_info_get_class_struct      (SCM scm_object_info);

G_GNUC_INTERNAL void
object_init                             (void);

#endif /* __OBJECT_H__ */
