#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

typedef unsigned long size_t;
typedef unsigned char byte;

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
   ACK = 0x04,
   NACK = 0x05,
};

#define BOOT_REQUEST 0x01
#define BOOT_REPLY 0x02
#define HARDWARE_ETHERNET 0x01
#define ETHERNET_LENGHT 0x06
#define DHCP_MagicCookie "\x63\x82\x53\x63"
#define REQUEST_END 0xff

int read_request(byte* buff, size_t buff_lenght, DhcpRequest *dhcp_request, char *client_ip);

#endif
