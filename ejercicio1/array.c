// Esto será una biblioteca dinámica
// Implementación de las funciones para que los clientes envíen peticiones

#include "array.h"

int init(char *name, int N){
  return 0;


}

int set(char *name, int i, int value){
  return 0;

  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //abrimos las colas de cliente y server
  char * clientName = (char *) getpid(); //Se le da un nombre para la cola del cliente
  int cq = mq_open( clientName , O_CREATE);
  if (cq == -1)
    return cq;
  int sq = mq_open("NOMBRE DE COLA SERVER"); //POR DEFINIR
  if (sq == -1){
    mq_close(cq);
    mq_unlink(clientName);
    return sq;
  }

  //Rellenamos la petición
  p.op = SET_OP;
  strcpy(p.name, name);
  strcpy(p.client_queue, clientName);
  p.value = value;

  //Enviamos la petición
  mq_send(sq, &p, ....);

  e = mq_receive(cq, &r, ...)
//--------------NO MUY SEGURO DE LO SIGUIENTE--------

  if (e == -1){
    mq_close(cq);
    mq_close(sq);
    mq_unlink(clientName);
    return e;
  }
    mq_close(cq);
    mq_close(sq);
    mq_unlink(clientName);
    return r.error;

}

int get(char *name, int i, int *value){
  return 0;
}

int destroy(chat *name){
  return 0;
}
