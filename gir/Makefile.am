lib_LTLIBRARIES = libgir-1.0.la
libgir_la_SOURCES = gir.c
libgir_la_CFLAGS = $(GUILE_CFLAGS) $(GIR_CFLAGS)
libgir_la_LIBADD = $(GUILE_LDFLAGS) $(GIR_LIBS)

schemedir = $(GUILE_SITE)
scheme_SCRIPTS = gir.scm

MAINTAINERCLEANFILES = Makefile.in
