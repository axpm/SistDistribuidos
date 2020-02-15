#ifndef COMM_HPP
#define COMM_HPP

#define MAX 50
// Códigos para las operaciones
#define INIT_OP 1
#define SET_OP 2
#define GET_OP 3
#define DEST_OP 4


struct petition{
  int op;
  char name [MAX];
  int N;
  int i;
  int value;
  char client_queue[MAX];
};

struct reply{
  int error; //Codigo de error 0 OK, -1 MAL
  int name;
  int i;
  int value;
};

#endif
