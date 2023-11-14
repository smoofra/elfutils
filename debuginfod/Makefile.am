## Makefile.am for libdebuginfod library subdirectory in elfutils.
##
## Process this file with automake to create Makefile.in
##
## Copyright (C) 2019 Red Hat, Inc.
## This file is part of elfutils.
##
## This file is free software; you can redistribute it and/or modify
## it under the terms of either
##
##   * the GNU Lesser General Public License as published by the Free
##     Software Foundation; either version 3 of the License, or (at
##     your option) any later version
##
## or
##
##   * the GNU General Public License as published by the Free
##     Software Foundation; either version 2 of the License, or (at
##     your option) any later version
##
## or both in parallel, as here.
##
## elfutils is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received copies of the GNU General Public License and
## the GNU Lesser General Public License along with this program.  If
## not, see <http://www.gnu.org/licenses/>.
##
include $(top_srcdir)/config/eu.am
AM_CPPFLAGS += -I$(srcdir) -I$(srcdir)/../libelf -I$(srcdir)/../libebl \
	   -I$(srcdir)/../libdw -I$(srcdir)/../libdwelf \
	   $(libmicrohttpd_CFLAGS) $(libcurl_CFLAGS) $(sqlite3_CFLAGS) \
	   $(libarchive_CFLAGS)

# Disable eu- prefixing for artifacts (binaries & man pages) in this
# directory, since they do not conflict with binutils tools.
program_prefix=
program_transform_name = s,x,x,

if BUILD_STATIC
libasm = ../libasm/libasm.a
libdw = ../libdw/libdw.a -lz $(zip_LIBS) $(libelf) $(libebl) -ldl -lpthread
libelf = ../libelf/libelf.a -lz
if DUMMY_LIBDEBUGINFOD
libdebuginfod = ./libdebuginfod.a
else
libdebuginfod = ./libdebuginfod.a -lpthread $(libcurl_LIBS)
endif
else
libasm = ../libasm/libasm$(SHEXT)
libdw = ../libdw/libdw$(SHEXT)
libelf = ../libelf/libelf$(SHEXT)
libdebuginfod = ./libdebuginfod$(SHEXT)
endif
libebl = ../libebl/libebl.a
libeu = ../lib/libeu.a
libdwelf = ../libdwelf/libdwelf.a
libdwfl = ../libdwfl/libdwfl.a

if GNU_LINKER
AM_LDFLAGS = -Wl,-rpath-link,../libelf:../libdw:.
endif

if APPLE_LINKER
AM_LDFLAGS = -Wl,-rpath,@executable_path/../lib
endif

bin_PROGRAMS =
if DEBUGINFOD
bin_PROGRAMS += debuginfod
endif

if LIBDEBUGINFOD
bin_PROGRAMS += debuginfod-find
endif

debuginfod_SOURCES = debuginfod.cxx
debuginfod_LDADD = $(libdw) $(libelf) $(libeu) $(libdebuginfod) $(argp_LDADD) $(fts_LIBS) $(libmicrohttpd_LIBS) $(sqlite3_LIBS) $(libarchive_LIBS) -lpthread -ldl

debuginfod_find_SOURCES = debuginfod-find.c
debuginfod_find_LDADD = $(libdw) $(libelf) $(libeu) $(libdwelf) $(libdwfl) $(libdebuginfod) $(argp_LDADD) $(fts_LIBS) $(zstd_LIBS) $(libdw) -lz $(LIBINTL)

if LIBDEBUGINFOD
noinst_LIBRARIES = libdebuginfod.a
noinst_LIBRARIES += libdebuginfod_pic.a
endif

libdebuginfod_a_SOURCES = debuginfod-client.c
libdebuginfod_pic_a_SOURCES = debuginfod-client.c
am_libdebuginfod_pic_a_OBJECTS = $(libdebuginfod_a_SOURCES:.c=.os)

if DUMMY_LIBDEBUGINFOD
AM_CPPFLAGS += -Wno-unused-parameter
endif

if LIBDEBUGINFOD
pkginclude_HEADERS = debuginfod.h
endif

if LIBDEBUGINFOD
libdebuginfod_so_LIBS = libdebuginfod_pic.a
if DUMMY_LIBDEBUGINFOD
libdebuginfod_so_LDLIBS =
else
libdebuginfod_so_LDLIBS = -lpthread $(libcurl_LIBS) $(fts_LIBS) $(libelf)
endif

if GNU_LINKER
WL_SONAME=-Wl,--soname,$(LIBDEBUGINFOD_SONAME)
endif

if SYMBOL_VERSIONING
WL_VERSION_SCRIPT=-WL,--version-script,$<
endif

if HOST_IS_ELF
TEXTREL_CHECK=$(textrel_check)
else
TEXTREL_CHECK=true
endif


$(LIBDEBUGINFOD_SONAME): $(srcdir)/libdebuginfod.map $(libdebuginfod_so_LIBS)
	$(AM_V_CCLD)$(LINK) $(dso_LDFLAGS) -o $@ \
		$(WL_SONAME) $(WL_VERSION_SCRIPT) \
		$(NO_UNDEFINED) \
		$(WHOLE_ARCHIVE) $(libdebuginfod_so_LIBS) $(NO_WHOLE_ARCHIVE) \
		$(libdebuginfod_so_LDLIBS)
	@$(TEXTREL_CHECK)

libdebuginfod$(SHEXT): $(LIBDEBUGINFOD_SONAME)
	ln -fs $< $@

install: install-am libdebuginfod$(SHEXT)
	$(mkinstalldirs) $(DESTDIR)$(libdir)
	$(INSTALL_PROGRAM) $(LIBDEBUGINFOD_SONAME) \
		$(DESTDIR)$(libdir)/libdebuginfod-$(PACKAGE_VERSION)$(SHEXT)
	ln -fs libdebuginfod-$(PACKAGE_VERSION)$(SHEXT) $(DESTDIR)$(libdir)/$(LIBDEBUGINFOD_SONAME)
	ln -fs libdebuginfod-$(PACKAGE_VERSION)$(SHEXT) $(DESTDIR)$(libdir)/libdebuginfod$(SHEXT)

uninstall: uninstall-am
	rm -f $(DESTDIR)$(libdir)/libdebuginfod-$(PACKAGE_VERSION)$(SHEXT)
	rm -f $(DESTDIR)$(libdir)/$(LIBDEBUGINFOD_SONAME)
	rm -f $(DESTDIR)$(libdir)/libdebuginfod$(SHEXT)
	rmdir --ignore-fail-on-non-empty $(DESTDIR)$(includedir)/elfutils
endif

EXTRA_DIST = libdebuginfod.map
MOSTLYCLEANFILES = $(am_libdebuginfod_pic_a_OBJECTS) $(LIBDEBUGINFOD_SONAME)
CLEANFILES += $(am_libdebuginfod_pic_a_OBJECTS) libdebuginfod$(SHEXT)

# automake std-options override: arrange to pass LD_LIBRARY_PATH
installcheck-binPROGRAMS: $(bin_PROGRAMS)
	bad=0; pid=$$$$; list="$(bin_PROGRAMS)"; for p in $$list; do \
	  case ' $(AM_INSTALLCHECK_STD_OPTIONS_EXEMPT) ' in \
	   *" $$p "* | *" $(srcdir)/$$p "*) continue;; \
	  esac; \
	  f=`echo "$$p" | \
	     sed 's,^.*/,,;s/$(EXEEXT)$$//;$(transform);s/$$/$(EXEEXT)/'`; \
	  for opt in --help --version; do \
	    if LD_LIBRARY_PATH=$(DESTDIR)$(libdir) \
	       $(DESTDIR)$(bindir)/$$f $$opt > c$${pid}_.out 2> c$${pid}_.err \
		 && test -n "`cat c$${pid}_.out`" \
		 && test -z "`cat c$${pid}_.err`"; then :; \
	    else echo "$$f does not support $$opt" 1>&2; bad=1; fi; \
	  done; \
	done; rm -f c$${pid}_.???; exit $$bad
