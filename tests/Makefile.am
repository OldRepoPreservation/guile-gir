EXTRA_DIST = call.scm

TESTS_ENVIRONMENT = LD_LIBRARY_PATH=$(top_srcdir)/gir/.libs \
		    GUILE_LOAD_PATH=$(top_srcdir)/gir $(GUILE) -s
TESTS = everything.scm

MAINTAINERCLEANFILES = Makefile.in
