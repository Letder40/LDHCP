#include <stdio.h>
#include <string.h>

#include "transactions.h"

int read_request(byte* buff, size_t buff_lenght, DhcpRequest *dhcp_request, char *client_ip) {
   if (buff[0] != BOOT_REQUEST) {
      fprintf(stderr, "[!] invalid dhcp request type, from %s\n", client_ip);
      return -1;
   }

   memcpy(dhcp_request->client_mac, &buff[28], 6);
   #ifdef DEBUG
   printf("Request from: %02x-%02x-%02x-%02x-%02x-%02x\n", dhcp_request->client_mac[0], dhcp_request->client_mac[1], dhcp_request->client_mac[2], dhcp_request->client_mac[3], dhcp_request->client_mac[4], dhcp_request->client_mac[5]);
   #endif

   if (memcmp(&buff[236], DHCP_MagicCookie, 4) != 0) {
      fprintf(stderr, "[!] invalid dhcp request magic cookie, from: %s\n", client_ip);
      return -1;
   }

   for (int i = 0; i < 4; i++)
      dhcp_request->transaction_id[i] = buff[4 + i];

   #ifdef DEBUG
   printf("Transaction ID: %02x-%02x-%02x-%02x\n", dhcp_request->transaction_id[0], dhcp_request->transaction_id[1], dhcp_request->transaction_id[2], dhcp_request->transaction_id[3]);
   #endif

   int readbuff_ptr = 240;
   int request_type_check = 0;
   for (int i = 0; 1; i++) {
      if (buff_lenght == readbuff_ptr) {
         fprintf(stderr, "[!] Request has execeded the allowed size %s\n", client_ip);
         return -1;
      }
      
      if (buff[readbuff_ptr] == REQUEST_END) {
         if (request_type_check == 0) {
            return -1;
         }
         return 0;
      }

      int id = buff[readbuff_ptr];
      int lenght = buff[++readbuff_ptr];

      if (id == 53 && lenght == 1) {
         request_type_check = 1;
         dhcp_request->request_type = buff[++readbuff_ptr];
         #ifdef DEBUG
         printf("request type: %u\n", dhcp_request->request_type);
         #endif
      }
      else if (id == 50 && lenght == 4) {
         #ifdef DEBUG
         printf("id -> 50\n");
         #endif
         dhcp_request->request_state = REQUESTED;
         for (int i = 0; i<4; i++) 
            dhcp_request->requested_addr[i] = buff[++readbuff_ptr];
      } 
      else {
         #ifdef DEBUG
         printf("id -> X \n");
         #endif
         for (int i; i < lenght; i++) 
            readbuff_ptr++;
      }
      readbuff_ptr++;
  }
}
