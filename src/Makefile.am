## Process this file with automake to create Makefile.in
##
## Copyright (C) 1996-2014, 2016 Red Hat, Inc.
## This file is part of elfutils.
##
## This file is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or
## (at your option) any later version.
##
## elfutils is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##
include $(top_srcdir)/config/eu.am
DEFS += $(YYDEBUG) -DDEBUGPRED=@DEBUGPRED@ \
	-DSRCDIR=\"$(shell cd $(srcdir);pwd)\" -DOBJDIR=\"$(shell pwd)\"
AM_CPPFLAGS += -I$(srcdir)/../libelf -I$(srcdir)/../libebl \
	    -I$(srcdir)/../libdw -I$(srcdir)/../libdwelf \
	    -I$(srcdir)/../libdwfl -I$(srcdir)/../libasm \
	    -I$(srcdir)/../gnulib

AM_LDFLAGS = $(STACK_USAGE_NO_ERROR)

if GNU_LINKER
AM_LDFLAGS += -Wl,-rpath-link,../libelf:../libdw
endif

if APPLE_LINKER
AM_LDFLAGS += -Wl,-rpath,@executable_path/../lib
endif

bin_PROGRAMS = readelf nm size strip elflint findtextrel addr2line \
	       elfcmp objdump ranlib strings ar unstrip stack elfcompress \
	       elfclassify srcfiles

noinst_LIBRARIES = libar.a

libar_a_SOURCES = arlib.c arlib2.c arlib-argp.c

EXTRA_DIST = arlib.h debugpred.h

bin_SCRIPTS = make-debug-archive
EXTRA_DIST += make-debug-archive.in
CLEANFILES += make-debug-archive

if BUILD_STATIC
libasm = ../libasm/libasm.a
libdw = ../libdw/libdw.a -lz $(zip_LIBS) $(libelf) -ldl -lpthread
libelf = ../libelf/libelf.a -lz $(zstd_LIBS)
else
libasm = ../libasm/libasm$(SHEXT)
libdw = ../libdw/libdw$(SHEXT)
libelf = ../libelf/libelf$(SHEXT)
endif
libebl = ../libebl/libebl.a ../backends/libebl_backends.a ../libcpu/libcpu.a
libeu = ../lib/libeu.a
gnulib = ../gnulib/libgnu.a

if DEMANGLE
demanglelib = -lstdc++
endif

# Bad, bad stack usage...
readelf_no_Wstack_usage = yes
nm_no_Wstack_usage = yes
size_no_Wstack_usage = yes
strip_no_Wstack_usage = yes
elflint_no_Wstack_usage = yes
elfcmp_no_Wstack_usage = yes
objdump_no_Wstack_usage = yes
ranlib_no_Wstack_usage = yes
ar_no_Wstack_usage = yes
unstrip_no_Wstack_usage = yes

readelf_LDADD = $(libdw) $(libebl) $(libelf) $(libeu) $(argp_LDADD) $(LIBINTL)
nm_LDADD = $(libdw) $(libebl) $(libelf) $(libeu) $(argp_LDADD) $(obstack_LIBS) \
	   $(demanglelib) $(LIBINTL) $(gnulib)
size_LDADD = $(libelf) $(libeu) $(argp_LDADD) $(LIBINTL)
strip_LDADD = $(libebl) $(libelf) $(libdw) $(libeu) $(argp_LDADD) $(LIBINTL) $(gnulib)
elflint_LDADD  = $(libebl) $(libdw) $(libelf) $(libeu) $(argp_LDADD) $(LIBINTL)
findtextrel_LDADD = $(libdw) $(libelf) $(libeu) $(argp_LDADD) $(LIBINTL)
addr2line_LDADD = $(libdw) $(libelf) $(libeu) $(argp_LDADD) $(demanglelib) $(LIBINTL)
elfcmp_LDADD = $(libebl) $(libdw) $(libelf) $(libeu) $(argp_LDADD) $(LIBINTL)
objdump_LDADD  = $(libasm) $(libebl) $(libdw) $(libelf) $(libeu) $(argp_LDADD) $(LIBINTL)
ranlib_LDADD = libar.a $(libelf) $(libeu) $(argp_LDADD) $(obstack_LIBS) $(LIBINTL) $(gnulib)
strings_LDADD = $(libelf) $(libeu) $(argp_LDADD) $(LIBINTL)
ar_LDADD = libar.a $(libelf) $(libeu) $(argp_LDADD) $(obstack_LIBS) $(LIBINTL) $(gnulib)
unstrip_LDADD = $(libebl) $(libelf) $(libdw) $(libeu) $(argp_LDADD) $(LIBINTL) $(gnulib)
stack_LDADD = $(libebl) $(libelf) $(libdw) $(libeu) $(argp_LDADD) $(demanglelib) $(LIBINTL)
elfcompress_LDADD = $(libebl) $(libelf) $(libdw) $(libeu) $(argp_LDADD) $(LIBINTL) $(gnulib)
elfclassify_LDADD = $(libelf) $(libdw) $(libeu) $(argp_LDADD) $(LIBINTL)
srcfiles_SOURCES = srcfiles.cxx
srcfiles_LDADD = $(libdw) $(libelf) $(libeu)  $(argp_LDADD) $(LIBINTL) $(gnulib)

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

CLEANFILES += *.gconv

make-debug-archive: $(srcdir)/make-debug-archive.in
	$(AM_V_GEN)UNSTRIP=$(bindir)/`echo unstrip | sed '$(transform)'`; \
	AR=$(bindir)/`echo ar | sed '$(transform)'`; \
	sed -e "s,[@]UNSTRIP[@],$$UNSTRIP,g" -e "s,[@]AR[@],$$AR,g" \
	    -e "s%[@]PACKAGE_NAME[@]%$(PACKAGE_NAME)%g" \
	    -e "s%[@]PACKAGE_VERSION[@]%$(PACKAGE_VERSION)%g" \
	    $(srcdir)/make-debug-archive.in > $@.new
	$(AM_V_at)chmod +x $@.new
	$(AM_V_at)mv -f $@.new $@
