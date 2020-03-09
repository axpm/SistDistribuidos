#ifndef SERVER_HPP
#define SERVER_HPP
#include "lines.h"
#include "imple.h"


void listenClient(int *cs);

void print_usage();
void printInitServer(struct sockaddr_in server_addr);

int registerUser(char * user);
int unregisterUser(char * user);
void serverMsg(char * msg);


#endif
