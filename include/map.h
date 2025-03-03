#ifndef MAP_H
#define MAP_H

#include <stddef.h>

struct map_utilities {
	unsigned int (*hash_algo)(const void *, size_t);
	void **(*index_func)(void **, int);
	void *(*single_dereference_func)(void **);
	void *(*insert_func)(void **, const void *);
	void *(*search_func)(void *, const void *);
};

void *insert_or_get_by_key(void **map, const void *key, size_t key_size,
                           struct map_utilities utils);
void *find_by_key(void **map, const void *key, size_t key_size,
                  struct map_utilities utils);
unsigned int adler32_string(const void *key, size_t key_size);
unsigned int adler32_voidp(const void *key, size_t key_size);

#endif

