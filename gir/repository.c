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

#include "repository.h"
#include "types.h"

/* SMOB types for GIRepository types */
scm_t_bits repository_t;
scm_t_bits typelib_t;

static SCM
scm_g_irepository_get_default (void)
{
        SCM repository = scm_make_smob (repository_t);
        SCM_SET_SMOB_DATA (repository, g_irepository_get_default ());

        return repository;
}

/* FIXME: scm_flags is currently ignored */
static SCM
scm_g_irepository_require (SCM scm_repository,
                           SCM scm_namespace,
                           SCM scm_version,
                           SCM scm_flags)
{
        GIRepository *repo;
        GTypelib *typelib;
        GError *error;
        const char *version;
        SCM scm_typelib;

        repo = (GIRepository *) SCM_SMOB_DATA (scm_repository);

        if (SCM_UNBNDP (scm_version)) {
                version = NULL;
        } else {
                version = scm_to_locale_string (scm_version);
        }

        error = NULL;
        typelib = g_irepository_require (repo,
                                         scm_to_locale_string (scm_namespace),
                                         version,
                                         0,
                                         &error);
        if (error) {
                /* FIXME: Throw Guile error here */
                g_critical ("Failed to load typelib: %s", error->message);
                return SCM_UNSPECIFIED;
        }

        scm_typelib = scm_make_smob (typelib_t);
        SCM_SET_SMOB_DATA (scm_typelib, typelib);

        return scm_typelib;
}

static SCM
scm_g_irepository_find_by_name (SCM scm_repository,
                                SCM scm_namespace,
                                SCM scm_name)
{
        GIRepository *repo;
        GIBaseInfo *info;
        GError *error;
        SCM scm_info;

        repo = (GIRepository *) SCM_SMOB_DATA (scm_repository);

        error = NULL;
        info = g_irepository_find_by_name (repo,
                                           scm_to_locale_string (scm_namespace),
                                           scm_to_locale_string (scm_name));

        scm_info = scm_make_smob (base_info_t);
        SCM_SET_SMOB_DATA (scm_info, info);

        return scm_info;
}

void
repository_init ()
{
        /* GIRepository */
        repository_t = scm_make_smob_type ("g-i-repository", 0);
        scm_c_define_gsubr ("g-irepository-get-default",
                            0,
                            0,
                            0,
                            scm_g_irepository_get_default);
        scm_c_define_gsubr ("g-irepository-require",
                            2,
                            2,
                            0,
                            scm_g_irepository_require);
        scm_c_define_gsubr ("g-irepository-find-by-name",
                            3,
                            0,
                            0,
                            scm_g_irepository_find_by_name);

        typelib_t = scm_make_smob_type ("g-typelib", 0);
}
