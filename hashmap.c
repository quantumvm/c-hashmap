#include "stdafx.h"
#include "hashmap.h"
#include "malloc.h"
#include <stdio.h>
#include <string.h>

int hash(char * key);
hash_map_bucket * getNextAvailableBucket(hash_map_bucket * lookedup);
hash_map_bucket * checkNextAvailableBucket(char * key, hash_map_bucket * bucket);


hash_map * hashmap_create(int num_buckets){
	//use foo as place holder so we can set number of buckets before returning
	hash_map * foo = (hash_map *) malloc(sizeof(hash_map));

	//allocate enough memory for pointers to buckets. Will later use with array notation
	//buckets[index]
	foo->buckets = (hash_map_bucket **) calloc(num_buckets, sizeof(hash_map_bucket *));
	foo->num_buckets = num_buckets;
	return foo;
}

void hashmap_put(char * key, void * value, hash_map * hashmap){
	
	//perform hashing to retrieve array index of bucket. Use bucket -1 because array indexing starts at 0
	int index = hash(key)%(hashmap->num_buckets-1);
	
	hash_map_bucket * lookedup = hashmap->buckets[index];
	//check to see if there is a collision (not null value). If there is get the next available bucket. Otherwise
	//just create a new bucket and assign the looked up value to it.
	if (lookedup != nullptr){
		hash_map_bucket * next = getNextAvailableBucket(lookedup);
				
		next->nextValue = (hash_map_bucket *) calloc(1, sizeof(hash_map_bucket));
		next->nextValue->value = value;
		next->nextValue->key = key;
	}
	else{
		hashmap->buckets[index] = (hash_map_bucket *) calloc(1, sizeof(hash_map_bucket));
		hashmap->buckets[index]->key = key;
		hashmap->buckets[index]->value = value;
	}

}



void * hashmap_get(char * key, hash_map * hashmap){
	void * value;
	int index = hash(key)%(hashmap->num_buckets - 1);
	
	if (hashmap->buckets[index] == nullptr){
		puts("VALUE IN HASHMAP COULD NOT BE FOUND!!!");
		return nullptr;
	}

	if (strcmp(key, hashmap->buckets[index]->key) == 0){
		value = hashmap->buckets[index]->value;
	}
	else{
		hash_map_bucket * collisionValue = checkNextAvailableBucket(key, hashmap->buckets[index]);
		if (collisionValue == nullptr){
			puts("VALUE IN HASHMAP COULD NOT BE FOUND!!!");
			return nullptr;
		}
		else{
			value = collisionValue->value;
		}
	}

	return value;
	 
	
}

//Recursive function used with @hashmap_put. Used to help handle collisions.
//Returns a node that already has a pointer to it. It's next value will be null.
hash_map_bucket * getNextAvailableBucket(hash_map_bucket * lookedup){
	if (lookedup->nextValue != nullptr){
		getNextAvailableBucket(lookedup->nextValue);
	}else{
		return lookedup;
	}
}

//Recursive function used with @hashmap_get checks the next bucket in a linked list if the key is not
//found in the initial collision.
hash_map_bucket * checkNextAvailableBucket(char * key ,hash_map_bucket * bucket){
	
	if (bucket->nextValue == nullptr){
		return nullptr;
	}
	else if (strcmp(key, bucket->nextValue->key)!=0){
		checkNextAvailableBucket(key, bucket->nextValue);
	}
	else{
		return bucket->nextValue;
	}
}


//Hash function adapted from Unix ELF hash
int hash(char * key){
	unsigned long h = 0, g;

	while (*key) {
		h = (h << 4) + *key++;
		if (g = h & 0xf0000000)
			h ^= g >> 24;
		h &= ~g;
	}

	return h;
}
