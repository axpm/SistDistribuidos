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
	pthread_attr_setdetachstate(&attrTh,PTHREAD_CREATE_DETACHED);
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
  //Leer del cliente operación
  err = readLine(clienteSd, buffer, MAX_LINE);
  if (err == -1) {
    perror("readLine");
  }
	// //to obtain the operation
	// char line[MAX_LINE];
	// memcpy (line, buffer, strlen(buffer)+1 );
	// char *operation = strtok(line, " ");

	//operaciones
  if (strcmp("QUIT", buffer) == 0){
		//enviar al cliente
    err = enviar(clienteSd, "", 1);
    if (err == -1) {
      perror("enviar");
    }
    close(clienteSd);
    pthread_exit(NULL);

  }else if (strcmp("REGISTER", buffer) == 0){
		char user[MAX_LINE] ;
		err = readLine(clienteSd, user, MAX_LINE);
	  if (err == -1) {
	    perror("readLine, user");
	  }

		printf("OPERATION FROM %s\n", user);
		printf("s> ");
		fflush(stdout);

		int reply = registerUser(user); //accion a realizar
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

		int reply = unregisterUser(user); //accion a realizar
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
	// else if (strcmp("CONNECT", buffer) == 0) {
	// 	char user[MAX_LINE];
	// 	err = readLine(clienteSd, user, MAX_LINE);
	// 	if (err == -1) {
	// 		perror("readLine, user");
	// 	}
	//
	// 	printf("OPERATION FROM %s\n", user);
	// 	printf("s> ");
	// 	fflush(stdout);
	//
	// 	//leer puerto del cliente
	// 	char port[MAX_LINE];
	// 	err = readLine(clienteSd, port, MAX_LINE);
	// 	if (err == -1) {
	// 		perror("readLine, file");
	// 	}
	//
	//
	// 	char ip[MAX_LINE];
	// 	struct hostent *host_entry;
	// 	gethostname(host, sizeof(host)); //find the host name
	// 	host_entry = gethostbyname(host); //find host information
	// 	ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); //Convert into IP string
	//
	// 	int reply = connect(user, port); //accion a realizar
	// 	//para poder enviar el código de error
	// 	char replyC[1];
	// 	bool cont = false;
	// 	switch (reply) {
	// 		case 0:
	// 			replyC[0] = '0';
	// 			cont = true;
	// 			break;
	// 		case 1:
	// 			replyC[0] = '1';
	// 			break;
	// 		default:
	// 			replyC[0] = '2';
	// 	}
	// 	err = enviar(clienteSd, replyC, 1);
	// 	if (err == -1) {
	// 		perror("enviar");
	// 	}
	//
	//
	// }//end of CONNECT



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

		//FIN

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

		int reply = publish(user, file, desc); //accion a realizar
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

		int reply = deleteContent(user, file); //accion a realizar
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
		int reply = list_users(user); //accion a realizar
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
				fillUserInfo(user, ip, port, &userLine, &nextUserLine, &noMore);

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
		int reply = list_content(user, userTarget); //accion a realizar
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

			findContentUser(userTarget, &firstLine, &lastLine);
			char file[MAX_LINE];

			for (int i = 0; i < n; i++){
				fillContentUser(file, &firstLine, lastLine, &noMore);

				if (noMore){ //envío de relleno
					enviar(clienteSd, "\0", 1);
				}else{ //envío normal de la info
					enviar(clienteSd, file, strlen(user)+1);
				}
			}
		}



	} //end LIST_CONTENT
  close(clienteSd);
  pthread_exit(NULL);

}
