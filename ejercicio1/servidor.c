#include "servidor.h"

int main(int argc, char const *argv[]) {
  struct peticion p;
  struct reply r;
  struct mq_attr attr;
  attr.mq_maxmsg = 10;

  int qs = mq_open(NOMBRE_SERVER, O_CREAT | O_WRONLY, &attr);
  if (qs == -1){
    fprintf(stderr, "%s\n", "Error! Not created Server Queue");
  }


  while(1){
    mq_receive(qs, (char *)p, sizeof(struct petition), NO_PRIORITY );

    //CREAR HILOS BAJO DEMANDA O COGER UNO DEL POOL

    //OPERACIONES
    //Operación Init
    if(p.op == INIT_OP){
      printf("%s\n", "Operación init");
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
