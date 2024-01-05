#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "transactions.h"
#include "../server/server.h"

#define MAX_BUFF 512

void get_lease_time(int lease_time, byte lease_time_bytes[4]) {
   lease_time_bytes[0] = lease_time >> 24 & 0xff;
   lease_time_bytes[1] = lease_time >> 16 & 0xff;
   lease_time_bytes[2] = lease_time >> 8 & 0xff;
   lease_time_bytes[3] = lease_time & 0xff;
}

time_t current_time_unix() {
   time_t current_time;
   time(&current_time);
   return current_time;
}

int get_next_addr(DhcpRequest dhcp_request, ServerData server_data, byte next_addr[4]) {
   PoolItem* pool_items = server_data.pool->pool_items; 

   if (!memcmp(dhcp_request.requested_addr, "\x00\x00\x00\x00", 4)) {
      for (int i = 0; i<server_data.pool->size; i++) {
         if (!memcmp(dhcp_request.requested_addr, pool_items[i].ip, 4) && (pool_items[i].state == FREE || !memcmp(dhcp_request.client_mac, pool_items->client_mac, 6))){
            memcpy(next_addr, pool_items[i].ip, 4);
            return i;
         }
      }
   }

   for (int i = 0; i<server_data.pool->size; i++) {
      if (pool_items[i].state == FREE) {
         memcpy(next_addr, pool_items[i].ip, 4);
         return i;
      }
   }

   return -1;
}

void response(int fd, enum DHCP_MessageType message_type, DhcpRequest dhcp_request, struct sockaddr_in addr, ServerData* server_data) { 
   byte next_addr[4];
   int index = get_next_addr(dhcp_request, *server_data, next_addr);
   
   if (index == -1) {
      return;
      fprintf(stderr, "DHCP pull is completed.\n");
   }

   byte response_buff[MAX_BUFF] = {0};
   int response_ptr = 0;
   response_buff[response_ptr++] = message_type; // OFFER || ACK
   response_buff[response_ptr++] = HARDWARE_ETHERNET; // HARWARDE TYPE
   response_buff[response_ptr++] = ETHERNET_LENGHT;
   response_buff[response_ptr++] = 0x00; // HOPS NOT IMPLEMENTED

   for (int i = 0; i < 4; i++)
      response_buff[response_ptr++] = dhcp_request.transaction_id[i];
   for (int i = 0; i < 2; i++)
      response_buff[response_ptr++] = 0x00; // SECONDS NOT IMPLEMENTED
   for (int i = 0; i < 2; i++)
      response_buff[response_ptr++] = 0x00; // BOTPFLAGS
   for (int i = 0; i < 4; i++)
      response_buff[response_ptr++] = 0x00; // CIADRR
   for (int i = 0; i < 4; i++)
      response_buff[response_ptr++] = next_addr[i]; // YIADDR
   for (int i = 0; i < 4; i++) 
      response_buff[response_ptr++] = server_data->server_addr[i]; // SADDR 
   for (int i = 0; i < 4; i++) 
      response_buff[response_ptr++] = 0x00; // RELAY AGENT NOT IMPLEMENTED
   for (int i = 0; i < 6; i++) 
      response_buff[response_ptr++] = dhcp_request.client_mac[i]; // CLIENT MAC ADDR
   for (int i = 0; i < 10; i++) 
      response_buff[response_ptr++] = 0x00; // 10 bytes padding for HWD_ADDR
   for (int i = 0; i < 64; i++) 
      response_buff[response_ptr++] = 0x00; // SERVER HOST NAME, not necessary 
   for (int i = 0; i < 128; i++) 
      response_buff[response_ptr++] = 0x00; // BOOT file (BOOTP), not needed in DHCP
   for (int i = 0; i < 4; i++)
      response_buff[response_ptr++] = DHCP_MagicCookie[i];

   // DHCP OPTIONS
   response_buff[response_ptr++] = 53; // DHCP MESSAGE TYPE
   response_buff[response_ptr++] = 1; // OPTION LEGHT
   response_buff[response_ptr++] = message_type;

   response_buff[response_ptr++] = 54;
   response_buff[response_ptr++] = 4;
   for(int i = 0; i < 4; i++)
      response_buff[response_ptr++] = server_data->server_addr[i]; 

   response_buff[response_ptr++] = 1; // SUBNETMASK OPTION
   response_buff[response_ptr++] = 4; // OPTION LEGHT
   for(int i = 0; i < 4; i++)
      response_buff[response_ptr++] = server_data->netmask[i]; 

   response_buff[response_ptr++] = 3; // DHCP DEFAULT GATEWAY
   response_buff[response_ptr++] = 4; // OPTION LEGHT
   for(int i = 0; i < 4; i++)
      response_buff[response_ptr++] = server_data->gateway[i]; 

   response_buff[response_ptr++] = 6; // DHCP DEFAULT DNS
   response_buff[response_ptr++] = 4; // OPTION LEGHT
   for(int i = 0; i < 4; i++)
      response_buff[response_ptr++] = server_data->dns_server[i]; 

   byte lease_time_bytes[4];
   response_buff[response_ptr++] = 51; // LEASE TIME
   response_buff[response_ptr++] = 4; // OPTION LEGHT
   get_lease_time(server_data->lease_time, lease_time_bytes);
   for(int i = 0; i < 4; i++)
      response_buff[response_ptr++] = lease_time_bytes[i]; 

   response_buff[response_ptr++] = 58; // LEASE TIME
   response_buff[response_ptr++] = 4; // OPTION LEGHT
   get_lease_time(server_data->lease_time * 0.5, lease_time_bytes);
   for(int i = 0; i < 4; i++)
      response_buff[response_ptr++] = lease_time_bytes[i]; 

   response_buff[response_ptr++] = 59; // RENEWAL
   response_buff[response_ptr++] = 4; // OPTION LEGHT
   get_lease_time(server_data->lease_time * 0.875, lease_time_bytes);
   for(int i = 0; i < 4; i++)
      response_buff[response_ptr++] = lease_time_bytes[i]; 
   
   response_buff[response_ptr++] = 0xff; // END
   response_buff[response_ptr++] = 0x00; // PADDING

   // POOL STATE CHANGE
   if (message_type == ACK) {
      PoolItem* pool_item = &server_data->pool->pool_items[index];
      pool_item->state = BUSY;
      pool_item->leased_in = current_time_unix();
      memcpy(pool_item->client_mac, dhcp_request.client_mac, 6);
   }

   size_t size = response_ptr;
   if (addr.sin_addr.s_addr == INADDR_ANY) {
      addr.sin_addr.s_addr = INADDR_BROADCAST;
   }
   sendto(fd, response_buff, size, 0, (struct sockaddr*)&addr, (unsigned int) sizeof(addr));
}
