void *insert_or_get_by_key(void **map, const void *key, size_t key_size,
                           struct map_utilities utils);
void *find_by_key(void **map, const void *key, size_t key_size,
                  struct map_utilities utils);
unsigned int adler32_string(const void *key, size_t key_size);
unsigned int adler32_voidp(const void *key, size_t key_size);
