#include "servidor.h"

int main(int argc, char const *argv[]) {
  struct petition p;
  struct reply r;
  struct mq_attr attr;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = sizeof(struct petition);
  int qs = mq_open(NOMBRE_SERVER, O_CREAT | O_RDWR, 0777, &attr);
  if (qs == -1){
    perror("mq_open");
    fprintf(stderr, "%s\n", "Error! Couldn't create Server Queue");
    exit(-1);
  }


  while(1){
    int e = mq_receive(qs, (char *)&p, sizeof(struct petition), NO_PRIORITY );

    if (e == -1){
      perror("mq_receive");
      mq_unlink(NOMBRE_SERVER);
      exit(-1); //Luego no habrá que hacer exit
    }


    //CREAR HILOS BAJO DEMANDA O COGER UNO DEL POOL

    //OPERACIONES
    //Operación Init
    if(p.op == INIT_OP){
      printf("%s\n", "Operación init");

      int qc = mq_open(p.client_queue, O_WRONLY);

      if (qc == -1){
        perror("mq_open");
        mq_unlink(NOMBRE_SERVER);
        exit(-1); //Luego no habrá que hacer exit
      }

      int e = mq_send(qc, (const char *)&r, sizeof(struct reply), NO_PRIORITY );
      if (e == -1){
        perror("mq_send");
        mq_unlink(NOMBRE_SERVER);
        exit(-1); //Luego no habrá que hacer exit
      }
      mq_unlink(p.client_queue);
      printf("%s\n", "Envía respuesta");
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
