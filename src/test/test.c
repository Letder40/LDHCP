#include <stdio.h>
#include "server/server.h"

int main() {
   ServerData server_data;
   server_configure(&server_data);
   server_data.pool = NULL;

   byte ip_addr[4] = {192,168,2,1};
   byte ip_addr2[4] = {192,168,2,2};
   byte ip_addr3[4] = {192,168,2,3};
   byte ip_addr4[4] = {192,168,2,4};
   byte ip_addr5[4] = {192,168,2,5};
   
   byte mac_addr1[6] = {0xa4, 0x43, 0xf3, 0xb2, 0xc1, 0xf1};
   byte mac_addr2[6] = {0xa4, 0x43, 0xf3, 0xb2, 0xc1, 0xf2};
   byte mac_addr3[6] = {0xa4, 0x43, 0xf3, 0xb2, 0xc1, 0xf3};
   byte mac_addr4[6] = {0xa4, 0x43, 0xf3, 0xb2, 0xc1, 0xf4};
   byte mac_addr5[6] = {0xa4, 0x43, 0xf3, 0xb2, 0xc1, 0xf5};
   
   PoolItem* item1 = pool_create_item(ip_addr, mac_addr1);
   PoolItem* item2 = pool_create_item(ip_addr2, mac_addr2);
   PoolItem* item3 = pool_create_item(ip_addr3, mac_addr3);
   PoolItem* item4 = pool_create_item(ip_addr4, mac_addr4);
   PoolItem* item5 = pool_create_item(ip_addr5, mac_addr5);

   pool_insert_item(&server_data.pool, item1);
   pool_insert_item(&server_data.pool, item2);
   pool_insert_item(&server_data.pool, item3);
   pool_insert_item(&server_data.pool, item4);
   pool_insert_item(&server_data.pool, item5);

   printf("TEST - INSERTING ITEMS:\n");
   pool_debug(server_data.pool);
   printf("\nEND TEST\n");

   pool_delete_item(server_data.pool, item4);

   printf("TEST - DELETING ITEMS:\n");
   pool_debug(server_data.pool);
   printf("\nEND TEST\n");

   pool_free_items(&server_data.pool);

   printf("TEST - FREE ITEMS:\n");
   pool_debug(server_data.pool);
   printf("\nEND TEST\n");
}
