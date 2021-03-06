#ifndef SERVER_HPP
#define SERVER_HPP
#include "lines.h"
#include "imple.h"
//Para el servicio rpc
#include "storage.h"



void listenClient(int *cs);

void print_usage();
void printInitServer(struct sockaddr_in server_addr);
void serverMsg(char * msg);

int lengthListUsers(t_listUsers l);
int lengthList(t_list l);

// int connectUser(user, ip, port);
// int registerUser(char * user);
// int unregisterUser(char * user);
// int publish(char *user, char *file, char *desc);
// int deleteContent(char *user, char *file);
// int list_users(char *user);
// int list_content(char *user, char *userTarget);
// int disconnectUser(char *user);

#endif
