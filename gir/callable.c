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
#include "types.h"

/* Symbol names for GITransfer values */
#define TRANSFER_NOTHING_SYMBOL "g-i-transfer-nothing"
#define TRANSFER_CONTAINER_SYMBOL "g-i-transfer-container"
#define TRANSFER_EVERYTHING_SYMBOL "g-i-transfer-everything"

/* SMOB types for CallableInfo & associated types */
scm_t_bits callable_info_t;
scm_t_bits callback_info_t;
scm_t_bits arg_info_t;

static SCM
scm_g_callable_info_get_return_type (SCM scm_callable_info)
{
        GICallableInfo *callable_info;
        GITypeInfo *type_info;
        SCM scm_return;

        callable_info = (GICallableInfo *) SCM_SMOB_DATA (scm_callable_info);
        type_info = g_callable_info_get_return_type (callable_info);
        if (type_info == NULL)
                return SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (type_info_t);
                SCM_SET_SMOB_DATA (scm_return, type_info);
        }

        return scm_return;
}

static SCM
scm_g_callable_info_get_caller_owns (SCM scm_callable_info)
{
        GICallableInfo *callable_info;

        callable_info = (GICallableInfo *) SCM_SMOB_DATA (scm_callable_info);

        scm_from_int (g_callable_info_get_caller_owns (callable_info));
}

static SCM
scm_g_callable_info_may_return_null (SCM scm_callable_info)
{
        GICallableInfo *callable_info;

        callable_info = (GICallableInfo *) SCM_SMOB_DATA (scm_callable_info);

        return scm_from_bool (g_callable_info_may_return_null (callable_info));
}

static SCM
scm_g_callable_info_get_n_args (SCM scm_callable_info)
{
        GICallableInfo *callable_info;

        callable_info = (GICallableInfo *) SCM_SMOB_DATA (scm_callable_info);

        return scm_from_int (g_callable_info_get_n_args (callable_info));
}

static SCM
scm_g_callable_info_get_arg (SCM scm_callable_info, SCM scm_n)
{
        GICallableInfo *callable_info;
        GIArgInfo *arg_info;
        SCM scm_return;

        callable_info = (GICallableInfo *) SCM_SMOB_DATA (scm_callable_info);
        arg_info = g_callable_info_get_arg (callable_info, scm_to_int (scm_n));
        if (arg_info == NULL)
                scm_return = SCM_BOOL_F;
        else {
                scm_return = scm_make_smob (arg_info_t);
                SCM_SET_SMOB_DATA (scm_return, arg_info);
        }

        return scm_return;
}

void
callable_init (void)
{
        callable_info_t = scm_make_smob_type ("g-i-callable-info", 0);
        callback_info_t = scm_make_smob_type ("g-i-callback-info", 0);
        arg_info_t = scm_make_smob_type ("g-i-arg-info", 0);

        scm_c_define_gsubr ("g-callable-info-get-return-type",
                            1,
                            0,
                            0,
                            scm_g_callable_info_get_return_type);
        scm_c_define_gsubr ("g-callable-info-get-caller-owns",
                            1,
                            0,
                            0,
                            scm_g_callable_info_get_caller_owns);
        scm_c_define_gsubr ("g-callable-info-may-return-null",
                            1,
                            0,
                            0,
                            scm_g_callable_info_may_return_null);
        scm_c_define_gsubr ("g-callable-info-get-n-args",
                            1,
                            0,
                            0,
                            scm_g_callable_info_get_n_args);
        scm_c_define_gsubr ("g-callable-info-get-arg",
                            2,
                            0,
                            0,
                            scm_g_callable_info_get_arg);

        scm_c_define (TRANSFER_NOTHING_SYMBOL,
                      scm_from_int (GI_TRANSFER_NOTHING));
        scm_c_define (TRANSFER_CONTAINER_SYMBOL,
                      scm_from_int (GI_TRANSFER_CONTAINER));
        scm_c_define (TRANSFER_EVERYTHING_SYMBOL,
                      scm_from_int (GI_TRANSFER_EVERYTHING));
}
