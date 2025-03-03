#include "map.h"
#include <stdint.h>

void*
insert_or_get_by_key(void **map, const void *key, size_t key_size,
                     struct map_utilities utils)
{
	void *tmp = find_by_key(map, key, key_size, utils);
	int hash;

	if (tmp)
		return tmp;
	hash = utils.hash_algo(key, key_size);
	return utils.insert_func(utils.index_func(map, hash), key);
}

void*
find_by_key(void **map, const void *key, size_t key_size, struct map_utilities utils)
{
	int hash = utils.hash_algo(key, key_size);
	return utils.search_func(utils.single_dereference_func(
	                           utils.index_func(map, hash)), key);
}

unsigned int
adler32_string(const void *key, size_t key_size)
{
	/* algoritm de hash creat de Mark Adler */
	const char *actual_key = (const char *)key;
	unsigned short s1 = 1, s2 = 0;
	int i;

	for (i = 0; i < key_size; i++) {
		s1 = (s1 + actual_key[i]) % 65521;
		s2 = (s2 + s1) % 65521;
	}
	return ((s1 << 16) | s2) % MAP_CONSTANT;
}

unsigned int
adler32_voidp(const void *key, size_t key_size)
{
	/* algoritm de hash creat de Mark Adler */
	uint8_t *actual_key = (uint8_t *)&key;
	unsigned short s1 = 1, s2 = 0;
	int i;

	for (i = 0; i < key_size; i++) {
		s1 = (s1 + actual_key[i]) % 65521;
		s2 = (s2 + s1) % 65521;
	}
	return ((s1 << 16) | s2) % MAP_CONSTANT;
}
