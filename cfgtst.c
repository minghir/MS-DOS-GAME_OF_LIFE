#include <dos.h>
#include "LVGA\\ini.h"

int main(){
int port;
char *server;

ini_t *config = ini_load("conf.ini");

const char *name = ini_get(config, "owner", "name");
if (name) {
  printf("name: %s\n", name);
}

ini_sget(config, "database", "server", NULL, &server);
ini_sget(config, "database", "port", "%d", &port);

printf("server: %s:%d\n", server, port);

ini_free(config);

sound(440);
delay(5);
nosound();



return 0;
}