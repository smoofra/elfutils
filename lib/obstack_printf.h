#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__APPLE__)

// gnulib's obstack_printf brings in gnulib's stdio.h, which brings with it a
// layer of compilcation and aggrivation that we don't need just for this
// function.  Implment it here the easy way with an extra copy.
#include "obstack.h"

static inline int
obstack_printf (struct obstack *obstack, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char *s = NULL;
  int n = vasprintf(&s, fmt, args);
  va_end(args);
  if (n >= 0) {
    obstack_grow0(obstack, s, n);
  }
  if (s) {
    free(s);
  }
  return n;
}

#endif
