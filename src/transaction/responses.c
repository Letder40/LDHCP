#include <netinet/in.h>
#include "transactions.h"
#include "../server/server.h"

#define MAX_BUFF 512


void dhcp_offer(int fd, DhcpRequest dhcp_request, struct sockaddr_in addr, ServerData* server_data) {
   byte response_buff[MAX_BUFF] = {0};
   int response_ptr = 0;
   response_buff[response_ptr++] = OFFER;
   response_buff[response_ptr++] = HARDWARE_ETHERNET;
   response_buff[response_ptr++] = ETHERNET_LENGHT;
   response_buff[response_ptr++] = 0; // HOPS NOT IMPLEMENTED
   for (int i = 0; i < 4; i++)
      response_buff[response_ptr++] = dhcp_request.transaction_id[i];
   for (int i = 0; i < 2; i++)
      response_buff[response_ptr++] = 0; // SECONDS NOT IMPLEMENTED
   for (int i = 0; i < 2; i++)
      response_buff[response_ptr++] = 0; // BOTPFLAGS
   for (int i = 0; i < 4; i++)
      response_buff[response_ptr++] = 0; // CLIENT ALLWAYS 0.0.0.0
   for (int i = 0; i < 4; i++)
      response_buff[response_ptr++] = 0;
}
