#include "server.h"

void server_configure(ServerData* server_config) {
   server_config->interface = "wlo1"; 
   byte first_addr[4] = {192, 168, 0, 200};
   byte last_addr[4] = {192, 168, 0, 254};
   for (int i = 0; i < 4; i++) server_config->first_addr[i] = first_addr[i];
   for (int i = 0; i < 4; i++) server_config->last_addr[i] = last_addr[i];
   for (int i = 0; i < 4; i++) server_config->next_addr[i] = first_addr[i];
   server_config->lease_time = 12 * 3600;
}
