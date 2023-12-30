#ifndef SERVER_H
#define SERVER_H

#include <time.h>

typedef unsigned char byte;
typedef unsigned long size_t;

typedef enum {
   FREE,
   BUSY,
} PoolItemState;

typedef struct PoolItem {
   PoolItemState state;
   byte client_mac[6];
   byte ip[4];
   time_t leased_in;
} PoolItem;

typedef struct {
   PoolItem* poolitems;
   size_t pool_size;
} Pool;

typedef struct {
   char* interface;
   Pool* pool;
   byte first_addr[4];
   byte last_addr[4];
   byte next_addr[4];
   byte netmask[4];
   byte gateway[4];
   byte dns_server[4];
   time_t lease_time;
} ServerData;

void server_configure(ServerData* server_config);

#endif
