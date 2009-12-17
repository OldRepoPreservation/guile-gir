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

#include <repository.h>
#include <types.h>
#include <registered-type.h>
#include <object.h>
#include <function.h>

void 
init ()
{
        g_type_init ();
        scm_c_eval_string ("(use-modules (gnome-2) (gnome gobject))");

        repository_init ();
        types_init ();
        registered_type_init ();
        object_init ();
        call_init ();
}

