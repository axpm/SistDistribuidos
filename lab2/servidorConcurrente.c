#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "lines.h"

#define MAX_LINE 	256

void listenClient(int *cs);

pthread_mutex_t mutex;
pthread_cond_t c;
bool copiado; //variable de condición

int main(int argc, char const *argv[]) {
  //Hilos
	pthread_attr_t attrTh;
  pthread_t t;
  pthread_attr_init(&attrTh);
	pthread_attr_setdetachstate(&attrTh,PTHREAD_CREATE_DETACHED);
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&c, NULL);
  copiado = false;
  //Sockets
  int sd, val, err, clienteSd;
  socklen_t size;
  struct sockaddr_in server_addr, client_addr;


  sd =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sd < 0) {
      perror("Error en socket");
      exit(1);
  }

  val = 1;
  err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));
  if (err < 0) {
      perror("Error en option");
      exit(1);
  }

  bzero((char *)&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(2500);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  /* bind */
  if (bind(sd,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    printf("Error en el bind\n");
    return(-1);
  }

  listen(sd, SOMAXCONN);

  printf("esperando conexion\n");
  while(1){

    //CREAR HILOS BAJO DEMANDA
    clienteSd = accept (sd, (struct sockaddr *) &client_addr, &size);
    printf("aceptada %s \n", inet_ntoa(client_addr.sin_addr) );
    if (clienteSd < 0) {
      printf("Error en el accept\n");
      return(-1);
    }
    if (pthread_create(&t, NULL, (void *) listenClient, &clienteSd) == -1){
      printf("Error creating threads\n");
      exit(0);
    }

    pthread_mutex_lock(&mutex);
    while (!copiado){
      pthread_cond_wait(&c, &mutex);
    }
    copiado = false;
    pthread_mutex_unlock(&mutex);

  }

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&c);
  close (sd);
  return(0);
}

void listenClient(int *cs){

  int clienteSd = *cs;
  //se ha recibido y copiado en petición
  pthread_mutex_lock(&mutex);
  copiado = true;
  pthread_cond_signal(&c);
  pthread_mutex_unlock(&mutex);

  char buffer[MAX_LINE];
  int err = 0;
  while (err != -1) {
    //Leer del cliente
    err = readLine(clienteSd, buffer, MAX_LINE);
    if (err == -1) {
      perror("readLine");
    }
    //enviar al cliente
    err = enviar(clienteSd, buffer, strlen(buffer)+1);
    if (err == -1) {
      perror("enviar");
    }

    if (strcmp("exit", buffer ) == 0){
      // close(clienteSd); //No muere, cierra la conexión
      // printf("cerrada %s \n", inet_ntoa(client_addr.sin_addr) ); //esto no va a funcionar, habría que pasarle los parámetros
      printf("%s\n", "Cerrada" );
      printf("esperando conexion\n");
      close(clienteSd);
      pthread_exit(NULL);
      break;
    }
  }
  close(clienteSd);
  pthread_exit(NULL);

}
