#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

typedef unsigned long size_t;
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
#define BOOT_REPLY 0x02
#define HARDWARE_ETHERNET 0x01
#define ETHERNET_LENGHT 0x06
#define REQUEST_END 0xff
#define DHCP_MagicCookie "\x63\x82\x53\x63"

int read_request(byte* buff, size_t buff_lenght, DhcpRequest *dhcp_request, char *client_ip);

#endif
