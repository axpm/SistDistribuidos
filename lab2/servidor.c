#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "lines.h"

#define MAX_LINE 	256



int main(int argc, char *argv[]) {
  int sd, val, err, clienteSd;
  socklen_t size;
  struct sockaddr_in server_addr, client_addr;
  char buffer[MAX_LINE];


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

  for (;;) {
    printf("esperando conexion\n");
    clienteSd = accept (sd, (struct sockaddr *) &client_addr, &size);
    printf("aceptada %s \n", inet_ntoa(client_addr.sin_addr) );
    if (clienteSd < 0) {
      printf("Error en el accept\n");
      return(-1);
    }

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
        close(clienteSd); //No muere, cierra la conexión
        break;
      }
    }
    close(clienteSd);
  } //end for

  close (sd);
  return(0);
}
