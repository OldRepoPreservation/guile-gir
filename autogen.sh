#! /bin/sh
mkdir -p m4
autoreconf -v --install || exit 1
glib-gettextize --force --copy || exit 1
./configure "$@"
