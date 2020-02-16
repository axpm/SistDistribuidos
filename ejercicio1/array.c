// Esto será una biblioteca dinámica
// Implementación de las funciones para que los clientes envíen peticiones

#include "array.h"

int init(char *name, int N){
  //return 0;

  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //abrimos las colas de cliente y server
  char * clientName = (char *) getpid(); //Se le da un nombre para la cola del cliente
    //Atributos cola
  mq_attr attr;
  attr.mq_maxmsg = MAX_MSS;
  attr.mq_msgsize = sizeof(reply);

  int cq = mq_open( clientName , O_CREAT | O_RDONLY, 0777, *attr);
  if (cq == -1)
    return cq;

  int sq = mq_open(NOMBRE_SERVER); //POR DEFINIR
  if (sq == -1){
    mq_close(cq);
    mq_unlink(clientName);
    return sq;
  }

  //Rellenamos la petición
  p.op = INIT_OP;
  strcpy(p.name, name);
  p.N = N;

  //Enviamos la petición
  mq_send(sq, &p, sizeof(petition), NO_PRIORITY);

  e = mq_receive(cq, &r, sizeof(reply), NO_PRIORITY);
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

int set(char *name, int i, int value){
  // return 0;

  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //abrimos las colas de cliente y server
  char * clientName = (char *) getpid(); //Se le da un nombre para la cola del cliente
  mq_attr attr;
  attr.mq_maxmsg = MAX_MSS;
  attr.mq_msgsize = sizeof(reply);

  int cq = mq_open( clientName , O_CREAT | O_RDONLY, 0777, *attr);
  if (cq == -1)
    return cq;

  int sq = mq_open(NOMBRE_SERVER); //POR DEFINIR
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
  mq_send(sq, &p, sizeof(petition), NO_PRIORITY);

  e = mq_receive(cq, &r, sizeof(reply), NO_PRIORITY);
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
  // return 0;

  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //abrimos las colas de cliente y server
  char * clientName = (char *) getpid(); //Se le da un nombre para la cola del cliente
  mq_attr attr;
  attr.mq_maxmsg = MAX_MSS;
  attr.mq_msgsize = sizeof(reply);

  int cq = mq_open( clientName , O_CREAT | O_RDONLY, 0777, *attr); //cliente queue = cq
  if (cq == -1)
    return cq;

  int sq = mq_open(NOMBRE_SERVER); //POR DEFINIR EL NOMBRE
  if (sq == -1){
    mq_close(cq);
    mq_unlink(clientName);
    return sq;
  }

  //Rellenamos la petición
  p.op = GET_OP;
  strcpy(p.name, name);
  strcpy(p.client_queue, clientName);
  p.i = i;

  //Enviamos la petición
  mq_send(sq, &p, sizeof(petition), NO_PRIORITY);

  e = mq_receive(cq, &r, sizeof(reply), NO_PRIORITY);
//--------------NO MUY SEGURO DE LO SIGUIENTE--------

  if (e == -1){
    mq_close(cq);
    mq_close(sq);
    mq_unlink(clientName);
    return e;
  }
    *value = r.value;
    mq_close(cq);
    mq_close(sq);
    mq_unlink(clientName);
    return r.error;
}

int destroy(chat *name){
  // return 0;

  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //abrimos las colas de cliente y server
  char * clientName = (char *) getpid(); //Se le da un nombre para la cola del cliente
  mq_attr attr;
  attr.mq_maxmsg = MAX_MSS;
  attr.mq_msgsize = sizeof(reply);

  int cq = mq_open( clientName , O_CREAT | O_RDONLY, 0777, *attr); //cliente queue = cq
  if (cq == -1)
    return cq;

  int sq = mq_open(NOMBRE_SERVER); //POR DEFINIR EL NOMBRE
  if (sq == -1){
    mq_close(cq);
    mq_unlink(clientName);
    return sq;
  }

  //Rellenamos la petición
  p.op = DEST_OP;
  strcpy(p.name, name);

  //Enviamos la petición
  mq_send(sq, &p, sizeof(petition), NO_PRIORITY);

  e = mq_receive(cq, &r, sizeof(reply), NO_PRIORITY);
//--------------NO MUY SEGURO DE LO SIGUIENTE--------

  if (e == -1){
    mq_close(cq);
    mq_close(sq);
    mq_unlink(clientName);
    return e;
  }
    *value = r.value;
    mq_close(cq);
    mq_close(sq);
    mq_unlink(clientName);
    return r.error;
}
