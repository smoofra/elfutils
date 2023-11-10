#include <search.h>
#include "system.h"

struct tdestroy_context {
    void **rootp;
    int (*compare)(const void *, const void *);
    void (*free)(void *p);
};

__thread struct tdestroy_context *tdestroy_context;

static inline void
tdestroy_action(const void *nodep, VISIT which, int) {
    if (which == leaf || which == endorder) {
        void *key = *(void**)nodep;
        tdelete(key, tdestroy_context->rootp, tdestroy_context->compare);
        tdestroy_context->free(key);
    }
}

void portable_tdestroy(void **rootp, void (*free)(void *nodep), int(*compare)(const void*, const void*)) {
    struct tdestroy_context ctx = {
        .rootp = rootp,
        .free = free,
        .compare = compare,
    };
    struct tdestroy_context *saved = tdestroy_context;
    tdestroy_context = &ctx;
    twalk(*rootp, tdestroy_action);
    tdestroy_context = saved;
};
