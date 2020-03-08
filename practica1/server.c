#include "server.h"

pthread_mutex_t mutex;
pthread_cond_t c;
bool copiado; //variable de condición

int main(int argc, char *argv[]) {
	//avoid being killed by a child

	//Hilos
	pthread_attr_t attrTh;
	pthread_t t;
	pthread_attr_init(&attrTh);
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&c, NULL);
	copiado = false;
	//Sockets
	int sd, val, err, clienteSd;
	socklen_t size;
	struct sockaddr_in server_addr, client_addr;

	int  option = 0;
	char port[256]= "";

	//  INSERT SERVER CODE HERE
	while ((option = getopt(argc, argv,"p:")) != -1) {
		switch (option) {
		    	case 'p' :
				strcpy(port, optarg);
		    		break;
		    	default:
				print_usage();
		    		exit(-1);
		    }
	}
	if (strcmp(port,"")==0){
		print_usage();
		exit(-1);
	}

	//preparing sockets
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
  server_addr.sin_port = htons(atoi(port));
  server_addr.sin_addr.s_addr = INADDR_ANY;

  /* bind */
  if (bind(sd,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    printf("Error en el bind\n");
    return(-1);
  }

	printInitServer(server_addr);

  listen(sd, SOMAXCONN);

	while(1){

    //CREAR HILOS BAJO DEMANDA
		clienteSd = accept (sd, (struct sockaddr *) &client_addr, &size);
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

	return 0;
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
  //Leer del cliente
  err = readLine(clienteSd, buffer, MAX_LINE);
  if (err == -1) {
    perror("readLine");
  }
	//to obtain the operation
	char line[MAX_LINE];
	memcpy (line, buffer, strlen(buffer)+1 );
	char *operation = strtok(line, " ");

	//operaciones
  if (strcmp("QUIT", operation) == 0){
		//enviar al cliente
    err = enviar(clienteSd, "", 1);
    if (err == -1) {
      perror("enviar");
    }
    close(clienteSd);
    pthread_exit(NULL);

  }else if (strcmp("REGISTER", operation) == 0){
		char user[MAX_LINE] ; 	// loop through the string to extract all other tokens

		//Para obtener el usuario
		memcpy (line, buffer, strlen(buffer)+1);
		char *token = strtok(line, " ");
		while( token != NULL ) {
			memcpy (user, token, strlen(buffer)+1);
		  token = strtok(NULL, " ");
		}

		printf("OPERATION FROM %s\n", user);
		printf("s> ");
		fflush(stdout);

		int reply = registerUser(user); //accion a realizar
		//int reply = 0;
		//para poder enviar el código de error
		char replyC[2];
		switch (reply) {
			case 0:
				replyC[0] = '0';
				break;
			case 1:
				replyC[0] = '1';
				break;
			default:
				replyC[0] = '2';
		}
		replyC[1] = '\0';
		err = enviar(clienteSd, replyC, 1);
    if (err == -1) {
      perror("enviar");
    }

	}else if(strcmp("UNREGISTER", operation ) == 0) {
		char user[MAX_LINE] ; 	// loop through the string to extract all other tokens

		//Para obtener el usuario
		memcpy (line, buffer, strlen(buffer)+1 );
		char *token = strtok(line, " ");
		while( token != NULL ) {
			memcpy (user, token, strlen(buffer)+1 );
		  token = strtok(NULL, " ");
		}

		printf("OPERATION FROM %s\n", user);
		printf("s> ");
		fflush(stdout);

		int reply = unregisterUser(user); //accion a realizar
		//int reply = 0;
		//para poder enviar el código de error
		char replyC[2];
		switch (reply) {
			case 0:
				replyC[0] = '0';
				break;
			case 1:
				replyC[0] = '1';
				break;
			default:
				replyC[0] = '2';
		}
		replyC[1] = '\0';
		err = enviar(clienteSd, replyC, 1);
    if (err == -1) {
      perror("enviar");
    }

	}
  close(clienteSd);
  pthread_exit(NULL);

}

//OPERACIONES
int registerUser(char * user){
	return 0;
}

int unregisterUser(char * user){
	return 0;
}


// ZONA DE MENSAJES EN LA PANTALLA
void print_usage() {
	    printf("Usage: server -p puerto \n");
}

void printInitServer(struct sockaddr_in server_addr){

	char host[256];
	char *IP;
	struct hostent *host_entry;
	gethostname(host, sizeof(host)); //find the host name
	host_entry = gethostbyname(host); //find host information
	IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); //Convert into IP string
	printf("s> ");
	printf("init server %s:%d\n", IP, ntohs(server_addr.sin_port));
	printf("s> ");
	fflush(stdout);
	//Fin init server
}
