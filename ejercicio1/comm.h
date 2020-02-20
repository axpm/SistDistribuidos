#ifndef COMM_HPP
#define COMM_HPP
#include <mqueue.h>

#define MAX 120 //máximo de los char
#define MAX_MSG 10
#define NO_PRIORITY 0
// Códigos para las operaciones
const int INIT_OP = 1;
const int SET_OP = 2;
const int GET_OP = 3;
const int DEST_OP = 4;

const char NOMBRE_SERVER [MAX] = "/myServer";

struct petition{
  int op;
  char name [MAX];
  int N;
  int i;
  int value;
  char client_queue[MAX];
};

struct reply{
  int error;
  int name;
  int i;
  int value;
};

#endif
