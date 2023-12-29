#ifndef SERVER_H
#define SERVER_H

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

void server_configure(ServerData* server_config);

PoolItem* pool_search_mac(PoolItem* pool, byte mac_addr[6]);

PoolItem* pool_search_address(PoolItem* pool, byte ip_address[4]);

PoolItem* pool_search_outdated(ServerData server_data, PoolItem* pool);

PoolItem* pool_create_item(byte* ip_addr, byte* mac_addr);

void pool_free_items(PoolItem** pool);

void pool_delete_item(PoolItem* pool, PoolItem* todelete_item);

void pool_insert_item(PoolItem** pool, PoolItem* toinsert_item);

void pool_debug(PoolItem* pool);

#endif
