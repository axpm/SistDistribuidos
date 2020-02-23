#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "lines.h"

#define MAX_LINE 	256

int main(int argc, char *argv[]){

  int sd;
  struct sockaddr_in server_addr;
  struct hostent *hp;

  int err = 0 ;


  if (argc != 3) {
          printf("Uso: client <direccion_servidor> <puerto_servidor>\n");
          exit(0);
  }

  // se crea el socket
  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sd < 0){
          perror("Error en socket");
          exit(1);
  }

  // se prepara la dirección del servidor
  bzero((char *)&server_addr, sizeof(server_addr));
  hp = gethostbyname (argv[1]);
  if (hp == NULL) {
          perror("Error en gethostbyname");
          exit(1);
  }

  memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));

  // en primer lugar establecer la conexión
  if(connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    printf("Error en la conexión\n");
    return(-1);
  }else{
    printf("%s\n", "Conectado correctamente\n" );
  }

  while (err != -1) {
    char bufferOut [MAX_LINE];
    char bufferIn [MAX_LINE];

    //Leer de entrada estandar
		err = readLine(STDIN_FILENO, bufferOut, MAX_LINE);
    if (err == -1){
      perror("readLine");
      return -1;
    }
    //Enviar al servidor
    err = enviar(sd, bufferOut, strlen(bufferOut)+1);
    if (err == -1){
      perror("enviar");
      return -1;
    }
    //Leer del servidor
    err = readLine(sd, bufferIn, MAX_LINE);
    if(err == -1){
      perror("readLine");
    }

    if (strcmp(bufferIn, "exit" ) == 0 )
      break;
    else {
      //escribir por pantalla
      err = writeLine(STDOUT_FILENO, bufferIn, strlen(bufferIn)+1);
      if(err == -1){
        perror("writeLine");
      }
    }
	}


  close(sd);

  return(0);
}
