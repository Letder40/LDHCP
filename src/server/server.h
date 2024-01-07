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
   byte server_addr[4];
   PoolVector* pool;
   byte first_addr[4];
   byte last_addr[4];
   byte next_addr[4];
   byte netmask[4];
   byte gateway[4];
   byte dns_server[4];
   int lease_time;
} ServerData;

#include "../transaction/transactions.h"

void get_server_address(byte server_addr[4], char* server_ifname);

void server_configure(ServerData* server_config);

void server_check_pool(ServerData* server_data);

void server_remove_client(ServerData* server_data, DhcpRequest dhcp_request, struct sockaddr_in client_addr);

PoolVector* pool_create();

void pool_growth(PoolVector* pool_vector);

void pool_insert_item(PoolVector* pool_vector, PoolItem pool_item);

void pool_free(PoolVector* pool_vector);

#endif
