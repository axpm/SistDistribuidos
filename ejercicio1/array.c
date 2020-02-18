// Esto será una biblioteca dinámica
// Implementación de las funciones para que los clientes envíen peticiones

#include "array.h"

int init(char *name, int N){
  //return 0;

  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //abrimos las colas de cliente y server
    //Atributos cola
  struct mq_attr attr;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = sizeof(struct reply);
  char clientName[MAX];
  sprintf(clientName, "/Queue-%d", getpid()); //Se le da un nombre para la cola del cliente
  printf("%s\n", clientName );
  int qc = mq_open( clientName , O_CREAT|O_RDONLY, 0700, &attr);

  if (qc == -1){
    perror("mq_open");
    return qc;
  }

  int qs = mq_open(NOMBRE_SERVER, O_WRONLY);
  if (qs == -1){
    mq_close(qc);
    mq_unlink(clientName);
    perror("mq_open");
    return qs;
  }

  //Rellenamos la petición
  p.op = INIT_OP;
  strcpy(p.name, name);
  p.N = N;

  //Enviamos la petición
  int e = mq_send(qs, (const char *) &p, sizeof(struct petition), NO_PRIORITY);

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_send");
    return e;
  }
  r.error = 1;
  e = mq_receive(qc, (char*) &r, sizeof(struct reply), NO_PRIORITY);
//--------------NO MUY SEGURO DE LO SIGUIENTE--------

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_receive");
    return e;
  }
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    return r.error;

}

int set(char *name, int i, int value){
  // return 0;

  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //abrimos las colas de cliente y server
    //Atributos cola
  struct mq_attr attr;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = sizeof(struct reply);
  char clientName[MAX];
  sprintf(clientName, "/Queue-%d", getpid()); //Se le da un nombre para la cola del cliente
  int qc = mq_open( clientName , O_CREAT | O_RDONLY, 0700, &attr);
  if (qc == -1){
    perror("mq_open");
    return qc;
  }

  int qs = mq_open(NOMBRE_SERVER, O_WRONLY);
  if (qs == -1){
    mq_close(qc);
    mq_unlink(clientName);
    perror("mq_open");
    return qs;
  }

  //Rellenamos la petición
  p.op = SET_OP;
  strcpy(p.name, name);
  strcpy(p.client_queue, clientName);
  p.value = value;

  //Enviamos la petición
  int e = mq_send(qs, (const char *) &p, sizeof(struct petition), NO_PRIORITY);

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_send");
    return e;
  }

  e = mq_receive(qc, (char*) &r, sizeof(struct reply), NO_PRIORITY);
//--------------NO MUY SEGURO DE LO SIGUIENTE--------

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_receive");
    return e;
  }
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    return r.error;

}

int get(char *name, int i, int *value){
  // return 0;

  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //abrimos las colas de cliente y server
    //Atributos cola
  struct mq_attr attr;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = sizeof(struct reply);
  char clientName[MAX];
  sprintf(clientName, "/Queue-%d", getpid()); //Se le da un nombre para la cola del cliente
  int qc = mq_open( clientName , O_CREAT | O_RDONLY, 0700, &attr);
  if (qc == -1){
    perror("mq_open");
    return qc;
  }


  int qs = mq_open(NOMBRE_SERVER, O_WRONLY); //POR DEFINIR EL NOMBRE
  if (qs == -1){
    mq_close(qc);
    mq_unlink(clientName);
    perror("mq_open");
    return qs;
  }

  //Rellenamos la petición
  p.op = GET_OP;
  strcpy(p.name, name);
  strcpy(p.client_queue, clientName);
  p.i = i;

  //Enviamos la petición
  int e = mq_send(qs, (const char *) &p, sizeof(struct petition), NO_PRIORITY);

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_send");
    return e;
  }

  e = mq_receive(qc, (char*) &r, sizeof(struct reply), NO_PRIORITY);
//--------------NO MUY SEGURO DE LO SIGUIENTE--------

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_receive");
    return e;
  }
    *value = r.value;
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    return r.error;
}

int destroy(char *name){
  // return 0;

  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //abrimos las colas de cliente y server
    //Atributos cola
  struct mq_attr attr;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = sizeof(struct reply);
  char clientName[MAX];
  sprintf(clientName, "/Queue-%d", getpid()); //Se le da un nombre para la cola del cliente
  int qc = mq_open( clientName , O_CREAT | O_RDONLY, 0700, &attr);
  if (qc == -1){
    perror("mq_open");
    return qc;
  }


  int qs = mq_open(NOMBRE_SERVER, O_WRONLY); //POR DEFINIR EL NOMBRE
  if (qs == -1){
    mq_close(qc);
    mq_unlink(clientName);
    perror("mq_open");
    return qs;
  }

  //Rellenamos la petición
  p.op = DEST_OP;
  strcpy(p.name, name);

  //Enviamos la petición
  int e = mq_send(qs, (const char *) &p, sizeof(struct petition), NO_PRIORITY);

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_send");
    return e;
  }

  e = mq_receive(qc, (char*) &r, sizeof(struct reply), NO_PRIORITY);
//--------------NO MUY SEGURO DE LO SIGUIENTE--------

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_receive");
    return e;
  }
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    return r.error;
}
