#include <stdlib.h>
#include "server.h"

PoolVector* pool_create() {
   PoolVector* pool_vector = (PoolVector*) malloc(sizeof(PoolVector));
   pool_vector->capacity = 1;
   pool_vector->size = 0;
   pool_vector->pool_items = malloc(sizeof(PoolItem));

   return pool_vector;
}

void pool_growth(PoolVector* pool_vector) {
   pool_vector->capacity *= 2;
   pool_vector->pool_items = realloc(pool_vector->pool_items, sizeof(PoolItem) * pool_vector->capacity);
} 

void pool_insert_item(PoolVector* pool_vector, PoolItem pool_item) {
   if (pool_vector->size == pool_vector->capacity) {
      pool_growth(pool_vector);
   }
   pool_vector->pool_items[pool_vector->size] = pool_item;
   pool_vector->size++;
}

void pool_free(PoolVector* pool_vector) {
   free(pool_vector->pool_items);
   free(pool_vector);
}
