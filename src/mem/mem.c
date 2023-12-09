#include "../../include/mem/mem.h"
#include "../../include/assert.h"
#include <stdlib.h>

static size_t mem_usage = 0;

char *mem_alloc(size_t size) {
    size_t align_size = ((size + sizeof(char*)) >> 2) << 2;
    mem_usage += align_size;
    char *ptr = malloc(align_size);
    ASSERT_NOT_NULL(ptr);
    return ptr;
}

void mem_free(char *ptr) {
    free(ptr);
}

size_t mem_get_usage() {
    return mem_usage;
}