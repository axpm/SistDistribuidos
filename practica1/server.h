#ifndef SERVER_HPP
#define SERVER_HPP
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <signal.h>
#include "lines.h"

#define MAX_LINE 	256

void print_usage();

void listenClient(int *cs);

void printInitServer(struct sockaddr_in server_addr);

int registerUser(char * user);
int unregisterUser(char * user);


#endif
