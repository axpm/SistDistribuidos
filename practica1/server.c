#include "server.h"

pthread_mutex_t mutex;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;
pthread_cond_t c;
pthread_cond_t c2;
bool copiado; //variable de condición

struct sockaddr_in client_addr; //accesible desde cualquier hilo, no es recomendable

int sd; //servidor accesible para todos
int nt = 0; //number of threads

void sigint_handler(int sig) {
	//compureba el estado del contador de hilos
	pthread_mutex_lock(&mutex3);
	while(nt > 0)
		pthread_cond_wait(&c2, &mutex3);
	pthread_mutex_unlock(&mutex3);

	//si han acabado todos los hilos ya acaba
	close(sd);
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex2);
	pthread_mutex_destroy(&mutex3);
	pthread_cond_destroy(&c);
	pthread_cond_destroy(&c2);
	printf("%s\n", "\nServer Closed" );
	exit(0);
}


int main(int argc, char *argv[]) {
	//avoid being killed by a child

	//Hilos
	pthread_attr_t attrTh;
	pthread_t t;
	pthread_attr_init(&attrTh);
	pthread_attr_setdetachstate(&attrTh, PTHREAD_CREATE_DETACHED);
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_mutex_init(&mutex3, NULL);
	pthread_cond_init(&c, NULL);
	pthread_cond_init(&c2, NULL);
	copiado = false;
	//Sockets
	int val, err, clienteSd;
	socklen_t size;
	// struct sockaddr_in server_addr, client_addr;
	struct sockaddr_in server_addr;

	int  option = 0;
	char port[256]= "";

	//Manejador de la señal ctrl-c
	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);

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
	close (sd);
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex2);
	pthread_mutex_destroy(&mutex3);
	pthread_cond_destroy(&c);
	pthread_cond_destroy(&c2);
	printf("%s\n", "\nServer Closed" );

	return 0;
}

void listenClient(int *cs){

	pthread_mutex_lock(&mutex3);
  nt ++;
  pthread_mutex_unlock(&mutex3);


  int clienteSd = *cs;
  //se ha recibido y copiado en petición
  pthread_mutex_lock(&mutex);
  copiado = true;
  pthread_cond_signal(&c);
  pthread_mutex_unlock(&mutex);


  char buffer[MAX_LINE];
  int err = 0;
  //Leer del cliente operación
  err = readLine(clienteSd, buffer, MAX_LINE);
  if (err == -1) {
    perror("readLine");
  }

  // if (strcmp("QUIT", buffer) == 0){
	// 	//enviar al cliente
  //   err = enviar(clienteSd, "", 1);
  //   if (err == -1) {
  //     perror("enviar");
  //   }
  //   close(clienteSd);
  //   pthread_exit(NULL);
	//
  // }else
	// --------------- OPERACIONES ---------------
	if (strcmp("REGISTER", buffer) == 0){
		char user[MAX_LINE] ;
		err = readLine(clienteSd, user, MAX_LINE);
	  if (err == -1) {
	    perror("readLine, user");
	  }

		printf("OPERATION FROM %s\n", user);
		printf("s> ");
		fflush(stdout);

		pthread_mutex_lock(&mutex2);
		int reply = registerUser(user); //accion a realizar
		pthread_mutex_unlock(&mutex2);
		//int reply = 0;
		//para poder enviar el código de error
		char replyC[1];
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
		err = enviar(clienteSd, replyC, 1);
    if (err == -1) {
      perror("enviar");
    }

	}else if(strcmp("UNREGISTER", buffer) == 0) {
		char user[MAX_LINE] ;
		err = readLine(clienteSd, user, MAX_LINE);
		if (err == -1) {
			perror("readLine, user");
		}

		printf("OPERATION FROM %s\n", user);
		printf("s> ");
		fflush(stdout);

		pthread_mutex_lock(&mutex2);
		int reply = unregisterUser(user); //accion a realizar
		pthread_mutex_unlock(&mutex2);
		//para poder enviar el código de error
		char replyC[1];
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
		err = enviar(clienteSd, replyC, 1);
    if (err == -1) {
      perror("enviar");
    }

	}//end of UNREGISTER


	// ----------------CONNECT sin hacer
	else if (strcmp("CONNECT", buffer) == 0) {
		char user[MAX_LINE];
		err = readLine(clienteSd, user, MAX_LINE);
		if (err == -1) {
			perror("readLine, user");
		}

		printf("OPERATION FROM %s\n", user);
		printf("s> ");
		fflush(stdout);

		//leer puerto del cliente
		char port[MAX_LINE];
		err = readLine(clienteSd, port, MAX_LINE);
		if (err == -1) {
			perror("readLine, file");
		}

		//get cliente ip
		struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client_addr;
		struct in_addr ipAddr = pV4Addr->sin_addr;
		char ip[INET_ADDRSTRLEN];
		// ip = inet_ntop(*(struct in_addr*) client_addr.sin_addr); //Convert into IP string
		inet_ntop( AF_INET, &ipAddr, ip, INET_ADDRSTRLEN );

		pthread_mutex_lock(&mutex2);
		int reply = connectUser(user, ip, port); //accion a realizar
		pthread_mutex_unlock(&mutex2);
		//para poder enviar el código de error
		char replyC[1];
		bool cont = false;
		switch (reply) {
			case 0:
				replyC[0] = '0';
				cont = true;
				break;
			case 1:
				replyC[0] = '1';
				break;
			default:
				replyC[0] = '2';
		}
		err = enviar(clienteSd, replyC, 1);
		if (err == -1) {
			perror("enviar");
		}

		if(cont){
			printf("%s\n", "" );
		}


	}//end of CONNECT



	else if(strcmp("PUBLISH", buffer) == 0){

		//NO LO PONE EN EL ENUNCIADO PERO ES LO LÓGICO
		//leer usuario que realiza la acción
		char user[MAX_LINE];
		err = readLine(clienteSd, user, MAX_LINE);
		if (err == -1) {
			perror("readLine, user");
		}

		printf("OPERATION FROM %s\n", user);
		printf("s> ");
		fflush(stdout);


		char file[MAX_LINE];
		err = readLine(clienteSd, file, MAX_LINE);
		if (err == -1) {
			perror("readLine, file");
		}

		char desc[MAX_LINE];
		err = readLine(clienteSd, desc, MAX_LINE);
		if (err == -1) {
			perror("readLine, description");
		}
		pthread_mutex_lock(&mutex2);
		int reply = publish(user, file, desc); //accion a realizar
		pthread_mutex_unlock(&mutex2);
		char replyC[1];
		switch (reply) {
			case 0:
				replyC[0] = '0';
				break;
			case 1:
				replyC[0] = '1';
				break;
			case 2:
				replyC[0] = '2';
				break;
			default:
				replyC[0] = '3';
		}

		err = enviar(clienteSd, replyC, 1);
		if (err == -1) {
			perror("enviar");
		}


	}//end of publish

	else if(strcmp("DELETE", buffer) == 0){
		char user[MAX_LINE];
		err = readLine(clienteSd, user, MAX_LINE);
		if (err == -1) {
			perror("readLine, user");
		}

		printf("OPERATION FROM %s\n", user);
		printf("s> ");
		fflush(stdout);

		char file[MAX_LINE];
		err = readLine(clienteSd, file, MAX_LINE);
		if (err == -1) {
			perror("readLine, file");
		}
		pthread_mutex_lock(&mutex2);
		int reply = deleteContent(user, file); //accion a realizar
		pthread_mutex_unlock(&mutex2);
			char replyC[1];
			switch (reply) {
				case 0:
					replyC[0] = '0';
					break;
				case 1:
					replyC[0] = '1';
					break;
				case 2:
					replyC[0] = '2';
					break;
				case 3:
					replyC[0] = '3';
					break;
				default:
					replyC[0] = '4';
			}

		err = enviar(clienteSd, replyC, 1);
    if (err == -1) {
      perror("enviar");
    }

	}//end of DELETE

	else if(strcmp("LIST_USERS", buffer) == 0){
		char user[MAX_LINE];
		err = readLine(clienteSd, user, MAX_LINE);
		if (err == -1) {
			perror("readLine, user");
		}

		printf("OPERATION FROM %s\n", user);
		printf("s> ");
		fflush(stdout);

		//comprueba si el user tiene problemas y envía respuesta
		pthread_mutex_lock(&mutex2);
		int reply = list_users(user); //accion a realizar
		pthread_mutex_unlock(&mutex2);
		//para poder enviar el código de error
		char replyC[1];
		bool cont = false;
		switch (reply) {
			case 0:
				replyC[0] = '0';
				cont = true;
				break;
			case 1:
				replyC[0] = '1';
				break;
			case 2:
				replyC[0] = '2';
				break;
			default:
				replyC[0] = '3';
		}
		err = enviar(clienteSd, replyC, 1);
    if (err == -1) {
      perror("enviar");
    }
		//si no dió error se leerá el número de users a imprimir
		if(cont){
			int n = 0;
			char nString[MAX_LINE];
			err = readLine(clienteSd, nString, MAX_LINE);
			if (err == -1) {
				perror("readLine, n");
			}
			n = atoi(nString);

			bool noMore = false;
			int userLine = 0, nextUserLine = 0;
			char ip[MAX_LINE];
			char port[MAX_LINE];

			for (int i = 0; i < n; i++){
				pthread_mutex_lock(&mutex2);
				fillUserInfo(user, ip, port, &userLine, &nextUserLine, &noMore);
				pthread_mutex_unlock(&mutex2);

				if (noMore){ //envío de relleno
					enviar(clienteSd, "\0", 1);
					enviar(clienteSd, "\0", 1);
					enviar(clienteSd, "\0", 1);
				}else{ //envío normal de la info de los usuarios
					enviar(clienteSd, user, strlen(user)+1);
					enviar(clienteSd, ip, strlen(ip)+1);
					enviar(clienteSd, port, strlen(port)+1);
				}

			}
		}


	} //end LIST_USERS
	else if (strcmp("LIST_CONTENT", buffer) == 0) {
		char user[MAX_LINE];
		err = readLine(clienteSd, user, MAX_LINE);
		if (err == -1) {
			perror("readLine, user");
		}

		printf("OPERATION FROM %s\n", user);
		printf("s> ");
		fflush(stdout);

		//envía acción y usuario objetivo
		char userTarget[MAX_LINE];
		err = readLine(clienteSd, userTarget, MAX_LINE);
		if (err == -1) {
			perror("readLine, user");
		}

		//comprueba si el user tiene problemas y envía respuesta
		pthread_mutex_lock(&mutex2);
		int reply = list_content(user, userTarget); //accion a realizar
		pthread_mutex_unlock(&mutex2);
		//para poder enviar el código de error
		char replyC[1];
		bool cont = false;
		switch (reply) {
			case 0:
			cont = true;
				replyC[0] = '0';
				break;
			case 1:
				replyC[0] = '1';
				break;
			case 2:
				replyC[0] = '2';
				break;
			case 3:
				replyC[0] = '3';
				break;
			default:
				replyC[0] = '4';
		}
		err = enviar(clienteSd, replyC, 1);
    if (err == -1) {
      perror("enviar");
    }
		//si no dió error se leerá el número de users a imprimir
		if(cont){
			int n = 0;
			char nString[MAX_LINE];
			err = readLine(clienteSd, nString, MAX_LINE);
			if (err == -1) {
				perror("readLine, n");
			}
			n = atoi(nString);

			bool noMore = false;
			int firstLine = 0, lastLine = 0;
			pthread_mutex_lock(&mutex2);
			findContentUser(userTarget, &firstLine, &lastLine);
			pthread_mutex_unlock(&mutex2);
			char file[MAX_LINE];

			for (int i = 0; i < n; i++){
				pthread_mutex_lock(&mutex2);
				fillContentUser(file, &firstLine, lastLine, &noMore);
				pthread_mutex_unlock(&mutex2);

				if (noMore){ //envío de relleno
					enviar(clienteSd, "\0", 1);
				}else{ //envío normal de la info
					enviar(clienteSd, file, strlen(user)+1);
				}
			}
		}

	} //end LIST_CONTENT

	close(clienteSd);

	//se disminuye el contador de hilos
	pthread_mutex_lock(&mutex3);
	nt--;
	pthread_cond_signal(&c2);
	pthread_mutex_unlock(&mutex3);

	pthread_exit(NULL);

}
