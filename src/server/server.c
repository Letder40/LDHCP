#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "server.h"

void server_create_pool(ServerData* config) {
   byte current_ip[4] = {0};
   memcpy(&current_ip, config->first_addr, 4);
   while (memcmp(&current_ip, config->last_addr, 4) != 0) {
      for (int i = 3; i >= 0; i--) {
         config->pool->pool_size++;
         if (++current_ip[i] != 0) {
            break;
         }
      }
   }
   
   config->pool->pool_size++;
   config->pool->poolitems = (PoolItem*) malloc(sizeof(PoolItem) * config->pool->pool_size);
   memcpy(&current_ip, config->first_addr, 4);
   int counter = 0;

   while (memcmp(&current_ip, config->last_addr, 4) != 0) {
      for (int i = 3; i >= 0; i--) {
         printf("IP: %u.%u.%u.%u\n", current_ip[0], current_ip[1], current_ip[2], current_ip[3]);
         PoolItem pool_item;
         memcpy(&pool_item.ip, current_ip, 4);
         pool_item.state = FREE;
         config->pool->poolitems[counter++] = pool_item;
         if (++current_ip[i] != 0) {
            break;
         }
      }
   }

   PoolItem pool_item;
   memcpy(&pool_item.ip, config->last_addr, 4);
   pool_item.state = FREE;
   config->pool->poolitems[counter] = pool_item;
}

void server_configure(ServerData* server_config) {
   Pool* pool = (Pool*) malloc(sizeof(Pool));
   server_config->pool = pool;
   server_config->interface = "wlo1"; 
   server_config->pool->pool_size = 0;
   byte first_addr[4] = {192, 168, 0, 10};
   byte last_addr[4] = {192, 168, 0, 100};
   for (int i = 0; i < 4; i++) server_config->first_addr[i] = first_addr[i];
   for (int i = 0; i < 4; i++) server_config->last_addr[i] = last_addr[i];
   server_config->lease_time = 12 * 3600;
   server_create_pool(server_config);
}


int main() {
   ServerData server_data;
   server_configure(&server_data);
   for (int i = 0; i < server_data.pool->pool_size; i++) {
      printf("--------------------------\n");
      printf("Pool Item: %d\n", i);
      printf("addr: %u.%u.%u.%u\n", server_data.pool->poolitems[i].ip[0],server_data.pool->poolitems[i].ip[1],server_data.pool->poolitems[i].ip[2],server_data.pool->poolitems[i].ip[3]);
      printf("state: %d\n", server_data.pool->poolitems[i].state);
   }
}
