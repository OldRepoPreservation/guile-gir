lib_LTLIBRARIES = libguile-gir-1.0.la
libguile_gir_1_0_la_SOURCES = gir.c \
			      repository.c \
			      repository.h \
			      types.c \
			      types.h \
			      call.c \
			      call.h \
			      object.c \
			      object.h
libguile_gir_1_0_la_CFLAGS = $(GUILE_CFLAGS) $(GIR_CFLAGS) $(GUILE_GLIB_CFLAGS)
libguile_gir_1_0_la_LIBADD = $(GUILE_LDFLAGS) $(GIR_LIBS) $(GUILE_GLIB_LIBS)

schemedir = $(GUILE_SITE)
scheme_SCRIPTS = gir.scm

MAINTAINERCLEANFILES = Makefile.in
