#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>

#include "server/server.h"
#include "transaction/transactions.h"

#define MAX_REQUEST_SIZE 2048
void handle_conn(int fd, ServerData* server_data) {
   while (1) {
      server_check_pool(server_data);

      struct sockaddr_in client_addr;
      socklen_t len = sizeof(client_addr);

      byte request_buff[MAX_REQUEST_SIZE] = {0};
      recvfrom(fd, request_buff, MAX_REQUEST_SIZE, 0, (struct sockaddr *)&client_addr, &len);

      char *client_ip = inet_ntoa(client_addr.sin_addr);
      DhcpRequest dhcp_request;

      if (read_request(request_buff, MAX_REQUEST_SIZE, &dhcp_request, client_ip) == -1) {
         continue;
      };

      switch (dhcp_request.request_type) {
         case DISCOVER:
            response(fd, OFFER, dhcp_request, client_addr, server_data);
            break;
         case REQUEST:
         case INFORM:
            response(fd, ACK, dhcp_request, client_addr, server_data);
            break;
         default:
            continue;
            break;
      }

      int buff_size = 1024;
      byte response_buff[buff_size];
      memset(request_buff, 0, buff_size);
   }
}

int serve(char* interface, unsigned int port) {
   struct sockaddr_in addr;
   addr.sin_addr.s_addr = INADDR_ANY;
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port);

   int fd = socket(AF_INET, SOCK_DGRAM, 0);
   int broadcast_enable = 1;
   if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) == -1) {
      perror("socket broadcast option: ");
   }
   
   if(setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, interface, strlen(interface)) == -1) {
      perror("socket bind_device option: ");
   } 

   if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {
      perror("bind error");
      exit(1);
   }
   return fd;
}

int main(int argc, char **argv) {
   ServerData server_data;
   server_configure(&server_data);

   int fd = serve(server_data.interface, 67);
   handle_conn(fd, &server_data);
}
