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
#define MAX_FILE_LINE 516 //file MAX_LINE *2 + delimitadores
#define DATABASE_NAME "database.db"

void print_usage();
void printInitServer(struct sockaddr_in server_addr);

int registerUserImple(char * user);
int unregisterUserImple(char * user);
int connectUserImple(char* user,char *ip, char *port);
int publishImple(char *user, char *file, char *desc);
int deleteContentImple(char *user, char *file);
int list_usersImple(char *user);
int list_contentImple(char *user, char *userTarget);
int disconnectUserImple(char *user);

void fillContentUserImple(char *file, int firstLine, int lastLine, bool *noMore);
void fillUserInfoImple(char *user, char * ip, char *port, int *userLine, int *nextUserLine, bool *noMore);

void serverMsg(char * msg);
int searchUserPos(FILE* fd, char *userFormat);
int searchNextUserPos(FILE* fd, int userLine);
int searchNextUserPosWithoutFD(FILE* fd, int userLine);
int deleteUser(FILE* fd, int userLine);
int validUsername(char *name);
int isConnected(FILE* fd, char *userFormat);
int isConnectedWithoutFD(FILE* fd, char *userFormat);
int searchFile(FILE* fd, char *file, int userLine, int nextUserLine);
int addFile(FILE* fd, char *fileFormat, int userLine, int nextUserLine);
int deleteFile(FILE* fd, int fileLine);
void findContentUser(char *user, int *firstLine, int* lastLine);


#endif
