#ifndef COMM_HPP
#define COMM_HPP
#include <mqueue.h>

#define MAX 120 //máximo de los char
#define MAX_MSG 10
#define NO_PRIORITY 0
// Códigos para las operaciones
#define INIT_OP 1
#define SET_OP 2
#define GET_OP 3
#define DEST_OP 4
// Nombre del servidor
#define NOMBRE_SERVER "/myServer"

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
