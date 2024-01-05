#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <netinet/in.h>

typedef unsigned long size_t;
typedef unsigned char byte;

#ifndef SERVER_H
#include "../server/server.h"
#endif

typedef enum  {
   REQUESTED,
   NO_REQUESTED,
} AddrRequest;

typedef struct {
   byte request_type;
   byte transaction_id[4];
   AddrRequest request_state;
   byte requested_addr[4];
   byte client_mac[6];
} DhcpRequest;

enum DHCP_MessageType {
   DISCOVER = 0x01,
   OFFER = 0x02,
   REQUEST = 0x03,
   NACK = 0x04,
   ACK = 0x05,
   DECLINE = 0x06,
   RELEASE = 0x07,
   INFORM = 0x08,
};

#define BOOT_REQUEST 0x01
#define BOOT_REPLY 0x02
#define HARDWARE_ETHERNET 0x01
#define ETHERNET_LENGHT 0x06
#define DHCP_MagicCookie "\x63\x82\x53\x63"
#define REQUEST_END 0xff

int read_request(byte* buff, size_t buff_lenght, DhcpRequest *dhcp_request, char *client_ip);

void response(int fd, enum DHCP_MessageType message_type, DhcpRequest dhcp_request, struct sockaddr_in addr, ServerData* server_data);

#endif
