#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "transactions.h"

int read_request(byte* buff, size_t buff_lenght, DhcpRequest *dhcp_request, char *client_ip) {
   if (buff[0] != BOOT_REQUEST) {
      fprintf(stderr, "[!] invalid dhcp request type, from %s\n", client_ip);
      return -1;
   }
   if (memcmp(&buff[236], DHCP_MagicCookie, 4) != 0) {
      fprintf(stderr, "[!] invalid dhcp request magic cookie, from: %s\n", client_ip);
      return -1;
   }

   for (int i = 0; i < 4; i++)
      dhcp_request->transaction_id[i] = buff[4 + i];

   dhcp_request->number_options = 0;
   int readbuff_ptr = 240;

   for (int i = 0; 1; i++) {
      if (buff_lenght == readbuff_ptr) {
         fprintf(stderr, "[!] Request has execeded the allowed size %s\n", client_ip);
         return -1;
      }
      
      if (buff[readbuff_ptr] == REQUEST_END) {
         break;
      }

      dhcp_request->options[i] = *(DhcpOption*) malloc(sizeof(DhcpOption));
      dhcp_request->number_options++;
      dhcp_request->options[i].id = buff[readbuff_ptr];
      dhcp_request->options[i].lenght = buff[++readbuff_ptr];
      dhcp_request->options[i].value = (byte*) malloc(sizeof(byte) * dhcp_request->options[i].lenght);

      fprintf(stderr, "id: %i\n", dhcp_request->options[i].id);

      for (int j = 0; j<dhcp_request->options[i].lenght; j++) {
         fprintf(stderr, "%c", buff[readbuff_ptr]);
         dhcp_request->options[i].value[j] = buff[++readbuff_ptr];
      }
      readbuff_ptr++;
  }
   return 0;
}
