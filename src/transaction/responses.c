#include <string.h>
#include <netinet/in.h>
#include "transactions.h"
#define MAX_BUFF 512

int get_option_index(DhcpRequest dhcp_request, int id) {
   int lenght = dhcp_request.number_options;
   if (lenght > 256) {
      return -1;
   }
   for (int i = 0; i < dhcp_request.number_options; i++) {
      if (dhcp_request.options[i].id == id) {
         return i;
      }
   }
   return -1;
}

void dhcp_offer(int fd, DhcpRequest dhcp_request, struct sockaddr_in addr) {
   int index_client_mac = get_option_index(dhcp_request, 61);
   int index_requested_addr = get_option_index(dhcp_request, 50);
   
   if (index_client_mac == -1) {
      return;
   }

   DhcpOption* option_client_mac = &dhcp_request.options[index_client_mac];
   if (option_client_mac->lenght != 0x06) {
      return;
   }

   byte client_mac[6];
   memcpy(client_mac, option_client_mac->value, 6);
}
