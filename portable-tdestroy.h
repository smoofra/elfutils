
#include <search.h>

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)

void
portable_tdestroy(void **rootp, void (*free)(void *nodep), int(*compare)(const void*, const void*));

#else

static inline void
portable_tdestroy(void **rootp, void (*free)(void *nodep), int(*compare)(const void*, const void*)) {
    tdestroy(*rootp);
}

#endif

