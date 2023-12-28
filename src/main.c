#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>

typedef unsigned char byte;

typedef struct {
   byte id;
   byte lenght;
   byte* value;
} DhcpOption;

#define OPTION_ARR_SIZE 256
typedef struct {
   byte transaction_id[4];
   int number_options;
   DhcpOption options[OPTION_ARR_SIZE];
} DhcpRequest;

enum DHCP_MessageType {
   DISCOVER = 0x01,
   OFFER = 0x02,
   REQUEST = 0x03,
   ACK = 0x04,
   NACK = 0x05,
};

#define BOOT_REQUEST 0x01
#define REQUEST_END 0xff
#define DHCP_MagicCookie "\x63\x82\x53\x63"


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

      switch (dhcp_request.options[0].value[0]) {
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

      printf("%d-%d-%d-%d\n", dhcp_request.transaction_id[0], dhcp_request.transaction_id[1], dhcp_request.transaction_id[2], dhcp_request.transaction_id[3]);

      // HERE
      int buff_size = 1024; // todo! get size from request_buff
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
