AC_INIT([Guile GIR], [0.1], [zeeshanak@gnome.org], [guile-gir])
AC_CONFIG_SRCDIR([gir/gir.scm])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

dnl Checks for programs.
AC_PROG_CC

dnl libtool stuff
AC_LIBTOOL_DLOPEN
dnl AC_LIBLTDL_CONVENIENCE
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)

dnl Check for guile
GUILE_FLAGS
GUILE_SITE_DIR

dnl Check for girepository
PKG_CHECK_MODULES(GIR, gobject-introspection-1.0 >= 0.6.5)
AC_SUBST(GIR_LIBS)
AC_SUBST(GIR_CFLAGS)

dnl Check for Guile 
PKG_CHECK_MODULES(GUILE, guile-2.0 >= 1.9.6)
AC_SUBST(GUILE_LIBS)
AC_SUBST(GUILE_CFLAGS)

dnl Check for Guile-GNOME's GLib
PKG_CHECK_MODULES(GUILE_GLIB, guile-gnome-glib-2 >= 2.16.1)
AC_SUBST(GUILE_GLIB_LIBS)
AC_SUBST(GUILE_GLIB_CFLAGS)

# Debugging
AC_ARG_ENABLE(debug,
	[  --enable-debug          enable debugging],,
        enable_debug=no)
if test "x$enable_debug" = "xyes"; then
        CFLAGS="$CFLAGS -g -Wall -Werror"
fi

dnl Done.
AC_CONFIG_FILES([Makefile
                 gir/Makefile
                 tests/Makefile])
AC_OUTPUT

