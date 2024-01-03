#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "server.h"

void get_server_address(byte server_addr[4], char* server_ifname) {
   struct ifreq ifr;
   ifr.ifr_addr.sa_family = AF_INET;

   int fd = socket(AF_INET, SOCK_DGRAM, 0);
   strncpy(ifr.ifr_name, server_ifname, IFNAMSIZ);
   ioctl(fd, SIOCGIFADDR, &ifr);
   
   char* iface_address_string = inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr);
   char* token;

   int counter = 0;
   token = strtok(iface_address_string, "."); 
   do {
      server_addr[counter] = (byte) atoi(token);
      counter++;
   }
   while ((token = strtok(NULL, ".")));
}

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
   server_data->lease_time = 12 * 3600;
   byte server_addr[4];
   byte first_addr[4] = {192, 168, 0, 1};
   byte last_addr[4] = {192, 168, 0, 255};
   get_server_address(server_addr, server_data->interface);
   memcpy(server_data->server_addr, server_addr, 4);
   memcpy(server_data->first_addr, first_addr, 4);
   memcpy(server_data->last_addr, last_addr, 4);
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
   //printf("server address: %u.%u.%u.%u\n",  server_data.server_addr[0], server_data.server_addr[1], server_data.server_addr[2], server_data.server_addr[3]);
}
