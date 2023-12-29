#ifndef __UIENGINE_MEM_H__
#define __UIENGINE_MEM_H__

#include <stddef.h>

char *mem_alloc(size_t size);

void mem_free(char *ptr);

size_t mem_get_usage();

#endif /* __UIENGINE_MEM_H__ */