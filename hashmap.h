#ifndef HASH_MAP
#define HASH_MAP

typedef struct hash_map_bucket{
	char * key;
	void * value;
	hash_map_bucket * nextValue;
}hash_map_bucket;

typedef struct hash_map{
	hash_map_bucket ** buckets;
	int num_buckets;
}hash_map;

hash_map * hashmap_create(int num_buckets);
void hashmap_put(char * key, void * value, hash_map * hashmap);
void * hashmap_get(char * key, hash_map * hashmap);


#endif