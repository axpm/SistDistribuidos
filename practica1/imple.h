#ifndef IMPLE_HPP
#define IMPLE_HPP

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

#define MAX_LINE 	256
#define MAX_FILE_LINE 514
#define DATABASE_NAME "database.db"

void print_usage();
void printInitServer(struct sockaddr_in server_addr);

int registerUser(char * user);
int unregisterUser(char * user);
int publish(char *user, char *file, char *desc);

void serverMsg(char * msg);
int searchUserPos(FILE* fd, char *userFormat);
int searchNextUserPos(FILE* fd, int userLine);
int deleteUser(FILE* fd, int userLine);
int validUsername(char *name);
int isConnected(FILE* fd, char *userFormat);
int searchFile(FILE* fd, char *file, int userLine, int nextUserLine);
void addFile(FILE* fd, char *fileFormat, int userLine, int nextUserLine);


#endif
