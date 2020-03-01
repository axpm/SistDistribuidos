// Implementación de las funciones para que los clientes envíen peticiones

#include "array.h"

//INIT
int init(char *name, int N){
  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //Atributos cola
  struct mq_attr attr;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = sizeof(struct reply);
  char clientName[MAX];
  sprintf(clientName, "/Queue-%d", getpid()); //Se le da un nombre único para la cola del cliente
  int qc = mq_open( clientName , O_CREAT|O_RDONLY, 0700, &attr); //Abrimos la cola del cliente

  if (qc == -1){
    perror("mq_open");
    return qc;
  }
  //Abrimos la cola del servidor
  int qs = mq_open(NOMBRE_SERVER, O_WRONLY);
  if (qs == -1){
    mq_close(qc);
    mq_unlink(clientName);
    perror("mq_open");
    return qs;
  }

  //Rellenamos la petición
  p.op = INIT_OP;
  sprintf(p.name, "%s", name);
  sprintf(p.client_queue, "%s", clientName);
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

  //Cerramos las colas del cliente y del servidor
  e = mq_receive(qc, (char*) &r, sizeof(struct reply), NO_PRIORITY);

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

//SET
int set(char *name, int i, int value){
  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //Atributos cola
  struct mq_attr attr;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = sizeof(struct reply);
  char clientName[MAX];
  sprintf(clientName, "/Queue-%d", getpid()); //Se le da un nombre único para la cola del cliente
  int qc = mq_open( clientName , O_CREAT | O_RDONLY, 0700, &attr); //Abrimos cola del cliente
  if (qc == -1){
    perror("mq_open");
    return qc;
  }

  //Abrimos cola del servidor
  int qs = mq_open(NOMBRE_SERVER, O_WRONLY);
  if (qs == -1){
    mq_close(qc);
    mq_unlink(clientName);
    perror("mq_open");
    return qs;
  }

  //Rellenamos la petición
  p.op = SET_OP;
  sprintf(p.name, "%s", name);
  sprintf(p.client_queue, "%s", clientName);
  p.value = value;
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

  //Recibimos la respuesta del servidor
  e = mq_receive(qc, (char*) &r, sizeof(struct reply), NO_PRIORITY);

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_receive");
    return e;
  }
  //Cerramos las colas del cliente y del servidor
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    return r.error;

}

//GET
int get(char *name, int i, int *value){
  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //Atributos cola
  struct mq_attr attr;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = sizeof(struct reply);
  char clientName[MAX];
  sprintf(clientName, "/Queue-%d", getpid()); //Se le da un nombre único para la cola del cliente
  int qc = mq_open( clientName , O_CREAT | O_RDONLY, 0700, &attr); //Creamos cola del cliente
  if (qc == -1){
    perror("mq_open");
    return qc;
  }

  //Abrimos la cola del servidor
  int qs = mq_open(NOMBRE_SERVER, O_WRONLY);
  if (qs == -1){
    mq_close(qc);
    mq_unlink(clientName);
    perror("mq_open");
    return qs;
  }

  //Rellenamos la petición
  p.op = GET_OP;
  sprintf(p.name, "%s", name);
  sprintf(p.client_queue, "%s", clientName);
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

  //Recibimos la respuesta del servidor
  e = mq_receive(qc, (char*) &r, sizeof(struct reply), NO_PRIORITY);

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_receive");
    return e;
  }
    *value = r.value;

    //Cerramos las colas del cliente y del servidor
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    return r.error;
}

//DESTROY
int destroy(char *name){
  //creamos las peticiones y respuestas
  struct petition p;
  struct reply r;

  //Atributos cola
  struct mq_attr attr;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = sizeof(struct reply);
  char clientName[MAX];
  sprintf(clientName, "/Queue-%d", getpid()); //Se le da un nombre único para la cola del cliente
  int qc = mq_open( clientName , O_CREAT | O_RDONLY, 0700, &attr);//Abrimos cola del cliente
  if (qc == -1){
    perror("mq_open");
    return qc;
  }

//Abrimos cola del servidor
  int qs = mq_open(NOMBRE_SERVER, O_WRONLY);
  if (qs == -1){
    mq_close(qc);
    mq_unlink(clientName);
    perror("mq_open");
    return qs;
  }

  //Rellenamos la petición
  p.op = DEST_OP;
  sprintf(p.name, "%s", name);
  sprintf(p.client_queue, "%s", clientName);

  //Enviamos la petición
  int e = mq_send(qs, (const char *) &p, sizeof(struct petition), NO_PRIORITY);

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_send");
    return e;
  }

  //Recibimos la respuesta del servidor
  e = mq_receive(qc, (char*) &r, sizeof(struct reply), NO_PRIORITY);

  if (e == -1){
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    perror("mq_receive");
    return e;
  }
  //Cerramos las colas del cliente y del servidor
    mq_close(qc);
    mq_close(qs);
    mq_unlink(clientName);
    return r.error;
}
