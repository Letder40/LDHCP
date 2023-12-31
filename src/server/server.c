#include <string.h>
#include <stdio.h>
#include "server.h"

void server_create_pool(ServerData* server_data) {
   byte current_ip[4] = {0};
   memcpy(&current_ip, server_data->first_addr, 4);

   while (memcmp(&current_ip, server_data->last_addr, 4) != 0) {
      for (int i = 3; i >= 0; i--) {
         //printf("IP: %u.%u.%u.%u\n", current_ip[0], current_ip[1], current_ip[2], current_ip[3]);
         PoolItem pool_item;
         memcpy(&pool_item.ip, current_ip, 4);
         pool_item.state = FREE;
         pool_insert_item(server_data->pool, pool_item);
         if (++current_ip[i] != 0) {
            break;
         }
      }
   }

   PoolItem pool_item;
   memcpy(&pool_item.ip, server_data->last_addr, 4);
   pool_item.state = FREE;
   pool_insert_item(server_data->pool, pool_item);
}

void server_configure(ServerData* server_data) {
   server_data->pool = pool_create();
   server_data->interface = "wlo1"; 
   byte first_addr[4] = {1, 1, 1, 1};
   byte last_addr[4] = {255, 255, 255, 254};
   for (int i = 0; i < 4; i++) server_data->first_addr[i] = first_addr[i];
   for (int i = 0; i < 4; i++) server_data->last_addr[i] = last_addr[i];
   server_data->lease_time = 12 * 3600;
   server_create_pool(server_data);
}


int main() {
   ServerData server_data;
   server_configure(&server_data);
   /*for (int i = 0; i < server_data.pool->size; i++) {
      printf("--------------------------\n");
      printf("Pool Item: %d\n", i);
      printf("addr: %u.%u.%u.%u\n", server_data.pool->pool_items[i].ip[0],server_data.pool->pool_items[i].ip[1],server_data.pool->pool_items[i].ip[2],server_data.pool->pool_items[i].ip[3]);
      printf("state: %d\n", server_data.pool->pool_items[i].state);
   }*/
   pool_free(server_data.pool);
}
