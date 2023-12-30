#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>

#include "transaction/transactions.h"

#define MAX_REQUEST_SIZE 2048
void handle_conn(int fd) {
   while (1) {
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
            printf("discover...\n");
            break;
         case REQUEST:
            printf("request...\n");
            break;
         default:
            continue;
            break;
      }

      int buff_size = 1024;
      byte response_buff[buff_size];
      memset(request_buff, 0, buff_size);
   }
   printf("valid ip_address sending response\n");
   // sendto(fd, response_buff, buff_size, 0, (struct sockaddr*)&client_addr, len);
}

int serve(in_addr_t s_addr, unsigned int port) {
   struct sockaddr_in addr;
   addr.sin_addr.s_addr = s_addr;
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port);

   int fd = socket(AF_INET, SOCK_DGRAM, 0);

   if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {
      perror("bind error");
      exit(1);
   }
   return fd;
}

int main(int argc, char **argv) {
   int fd = serve(INADDR_ANY, 67);
   handle_conn(fd);
}
