#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

PoolItem* pool_search_outdated(ServerData server_data, PoolItem* pool) {
   if (pool->next_item == NULL) {
      return NULL;
   }
   
   time_t current_time;
   time(&current_time);
   time_t leased_time = current_time - pool->leased_in;
   if (leased_time < server_data.lease_time) {
      return pool;
   }
   return pool_search_outdated(server_data, pool->next_item);
}

PoolItem* pool_search_mac(PoolItem* pool, byte mac_addr[6]) {
   if (pool->next_item == NULL) {
      return NULL;
   }
   if (memcmp(pool->client_mac, mac_addr, 6) == 0) {
      return pool;
   }
   return pool_search_mac(pool->next_item, mac_addr);
}

void pool_delete_item(PoolItem* pool, PoolItem* todelete_item) {
   if (pool->next_item == NULL) {
      return;
   }
   if (memcmp(pool->next_item->client_mac, todelete_item->client_mac, 6) == 0) {
      pool->next_item = todelete_item->next_item;
   } else {
      return pool_delete_item(pool->next_item, todelete_item);
   }
}

void pool_insert_item(PoolItem** pool, PoolItem* toinsert_item) {
   if (*pool == NULL) {
      *pool = toinsert_item;
   } else if ((*pool)->next_item == NULL) {
      (*pool)->next_item = toinsert_item;
      toinsert_item->next_item = NULL; 
   } else {
      return pool_insert_item(&(*pool)->next_item, toinsert_item);
   }
}

void pool_create_item(PoolItem* item, byte* ip_addr, byte* mac_addr) {
   for (int i = 0; i < 4; i++) item->ip[i] = ip_addr[i];
   for (int i = 0; i < 6; i++) item->client_mac[i] = mac_addr[i];

   time_t current_time;
   time(&current_time);
   item->leased_in = current_time;
   item->next_item = NULL;
}

void pool_debug(PoolItem* pool) {
   if (pool == NULL) {
      return;
   }

   fprintf(stderr, "pool - mac : %02x-%02x-%02x-%02x-%02x-%02x\n", pool->client_mac[0], pool->client_mac[1], pool->client_mac[2], pool->client_mac[3], pool->client_mac[4], pool->client_mac[5]);
   fprintf(stderr, "pool - ip : %u.%u.%u.%u\n", pool->ip[0], pool->ip[1], pool->ip[2], pool->ip[3]);
   fprintf(stderr, "pool - leased_in: %lu\n", pool->leased_in);

   if (pool->next_item != NULL) {
      return pool_debug(pool->next_item);
   } else {
      return;
   }
}
