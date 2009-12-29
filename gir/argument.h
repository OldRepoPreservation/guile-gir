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

#ifndef __ARGUMENT_H__
#define __ARGUMENT_H__

#include <glib.h>
#include <libguile.h>
#include <girepository.h>

extern scm_t_bits arg_info_t;

G_GNUC_INTERNAL SCM
gi_return_value_to_scm                  (GICallableInfo        *info,
                                         GArgument              return_value);

G_GNUC_INTERNAL SCM
gi_arg_to_scm                           (GITypeInfo            *arg_type,
                                         GITransfer             transfer_type,
                                         GArgument              arg);

G_GNUC_INTERNAL void
scm_to_gi_arg                           (SCM                    scm_arg,
                                         GIArgInfo             *arg_info,
                                         GArgument             *arg);

G_GNUC_INTERNAL SCM
gi_interface_to_scm                     (GITypeInfo            *arg_type,
                                         GITransfer             transfer_type,
                                         GArgument              arg);

G_GNUC_INTERNAL void
scm_to_gi_interface                     (SCM                    scm_arg,
                                         GIInfoType             arg_type,
                                         GITransfer             transfer_type,
                                         GArgument             *arg);

G_GNUC_INTERNAL void
argument_init                           (void);

#endif /* __ARGUMENT_H__ */
