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

typedef struct vector {
   size_t size; 
   size_t capacity; //real capacity
   PoolItem* pool_items;
} PoolVector;

typedef struct {
   char* interface;
   PoolVector* pool;
   byte first_addr[4];
   byte last_addr[4];
   byte next_addr[4];
   byte netmask[4];
   byte gateway[4];
   byte dns_server[4];
   time_t lease_time;
} ServerData;

void server_configure(ServerData* server_config);

//vector functions
PoolVector* pool_create();

void pool_growth(PoolVector* pool_vector);

void pool_insert_item(PoolVector* pool_vector, PoolItem pool_item);

void pool_free(PoolVector* pool_vector);

#endif
