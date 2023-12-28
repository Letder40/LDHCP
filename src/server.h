#include <time.h>

typedef unsigned char byte;
typedef unsigned long size_t;

typedef struct PoolItem {
   byte client_mac[6];
   byte ip[4];
   time_t leased_in;
   struct PoolItem* next_item;
} PoolItem;

typedef struct {
   char* interface;
   PoolItem* pool;
   byte first_addr[4];
   byte last_addr[4];
   byte next_addr[4];
   byte netmask[4];
   byte gateway[4];
   byte dns_server[4];
   time_t lease_time;
} ServerData;
