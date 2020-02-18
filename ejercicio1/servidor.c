#include "servidor.h"

int main(int argc, char const *argv[]) {
  struct petition p;
  // struct reply r;
  struct mq_attr attr;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = sizeof(struct petition);
  int qs = mq_open(NOMBRE_SERVER, O_CREAT | O_WRONLY, 0777, &attr);
  if (qs == -1){
    perror("mq_open");
    fprintf(stderr, "%s\n", "Error! Couldn't create Server Queue");
    exit(-1);
  }


  while(1){
    mq_receive(qs, (char *)&p, sizeof(struct petition), NO_PRIORITY );

    //CREAR HILOS BAJO DEMANDA O COGER UNO DEL POOL
    printf("%d %s\n", p.op, "Soy el server" );

    //OPERACIONES
    //Operación Init
    if(p.op == INIT_OP){
      printf("%s\n", "Operación init");
      exit(-1);
    }
    //Operación Set
    if(p.op == SET_OP){
      printf("%s\n", "Operación set");
    }
    //Operación Get
    if(p.op == GET_OP){
      printf("%s\n", "Operación get");
    }
    //Operación destroy
    if(p.op == DEST_OP){
      printf("%s\n", "Operación destroy");
    }
  }
  return 0;
}
