#include "servidor.h"

#define MAX_PETICIONES 256

struct petition buffer_peticiones[MAX_PETICIONES]; // buffer de peticiones
int n_elementos; // elementos en el buffer de peticiones
int pos_servicio = 0;

pthread_mutex_t mutex;
pthread_mutex_t mutex2;
pthread_cond_t no_lleno;
pthread_cond_t no_vacio;
pthread_mutex_t mfin;
pthread_t thid[MAX_THREADS];
bool fin = false;
bool finSignal = false;

void sigint_handler(int sig) {

	//Se pone fin a true
	pthread_mutex_lock(&mfin);
	fin=true;
	pthread_mutex_unlock(&mfin);
	pthread_mutex_lock(&mutex);
	pthread_cond_broadcast(&no_vacio);
	pthread_mutex_unlock(&mutex);

	for (int i=0;i<MAX_THREADS;i++){
			pthread_join(thid[i],NULL);
	}

	fprintf(stderr,"Finalizando servicio\n");

	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex2);
	pthread_cond_destroy(&no_lleno);
	pthread_cond_destroy(&no_vacio);
	pthread_mutex_destroy(&mfin);
	exit(0);
}


int main(int argc, char const *argv[]) {

	//Manejador de la finSignal
	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = SA_RESTART;
	// sigaction(SIGINT, &sa, NULL);

	//Atributos
	struct mq_attr attr;
	attr.mq_maxmsg = MAX_MSG;
	attr.mq_msgsize = sizeof(struct petition);

	//Cola servidor
	int qs = mq_open(NOMBRE_SERVER, O_CREAT | O_RDWR, 0777, &attr);
	if (qs == -1){
		perror("mq_open");
		fprintf(stderr, "%s\n", "Error! Couldn't create Server Queue");
		exit(-1);
	}

  //Hilos
	pthread_attr_t attrTh;
	pthread_attr_init(&attrTh);
	pthread_attr_setdetachstate(&attrTh, PTHREAD_CREATE_DETACHED);
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&no_lleno,NULL);
	pthread_cond_init(&no_vacio,NULL);
	pthread_mutex_init(&mfin,NULL);

	int error;
	int pos = 0;

	// Creando Pool
	for (int i = 0; i < MAX_THREADS; i++){
		if (pthread_create(&thid[i], NULL, (void *) servicio, NULL) !=0){
			perror("Error creando el pool de threads\n");
			return 0;
		}
	}

  while(1){
		struct petition p;
		error = mq_receive(qs, (char *) &p, sizeof(struct petition), 0);
		if (error == -1 )
			break;

		//Si el buffer está lleno, se espera a que haya hueco
		pthread_mutex_lock(&mutex);
		while (n_elementos == MAX_PETICIONES){
			pthread_cond_wait(&no_lleno, &mutex);
		}

		//Si hay hueco, se introduce una petición en el buffer y se aumenta en 1 el número de elementos
		buffer_peticiones[pos] = p;
		pos = (pos+1) % MAX_PETICIONES;
		n_elementos++;
		pthread_cond_signal(&no_vacio);
		pthread_mutex_unlock(&mutex);

	}

	//Se pone fin a true
	pthread_mutex_lock(&mfin);
	fin = true;
	pthread_mutex_unlock(&mfin);
	pthread_mutex_lock(&mutex);
	pthread_cond_broadcast(&no_vacio);
	pthread_mutex_unlock(&mutex);

	for (int i=0;i<MAX_THREADS;i++){
			pthread_join(thid[i],NULL);
	}
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex2);
	pthread_cond_destroy(&no_lleno);
	pthread_cond_destroy(&no_vacio);
	pthread_mutex_destroy(&mfin);


  return 0;
}

void* servicio(void ){
  struct petition p; //Petición
  struct reply r;//Respuesta

	for(;;){
		//Si no hay elementos en el buffer y fin está a true es porque se ha termiando y salimos
		//Si está a false, esperamos a que haya un elemento en el buffer
		pthread_mutex_lock(&mutex);
		while (n_elementos == 0) {
			if (fin==true) {
				// fprintf(stderr,"Finalizando servicio\n");
				pthread_mutex_unlock(&mutex);
				pthread_exit(0);
			}
			pthread_cond_wait(&no_vacio, &mutex);
		}
		//Si hay elementos, cogemos un elemento del buffer y restamos en 1 el número de elementos
		p = buffer_peticiones[pos_servicio];
		pos_servicio = (pos_servicio + 1) % MAX_PETICIONES;
		n_elementos --;
		pthread_cond_signal(&no_lleno);
		pthread_mutex_unlock(&mutex);


		/* ejecutar la petición del cliente y preparar respuesta */
		//OPERACIONES
	  //Operación Init
	  if(p.op == INIT_OP){
	    // printf("%s\n", "Operation init");
	    //ejecución y rellenar respuesta
			pthread_mutex_lock(&mutex2);
	    r.error = init(p.name, p.N);
			pthread_mutex_unlock(&mutex2);
	  }
	  //Operación Set
	  else if(p.op == SET_OP){
	    // printf("%s\n", "Operation set");
	    //ejecución y rellenar respuesta
			pthread_mutex_lock(&mutex2);
			r.error = set(p.name, p.i, p.value);
			pthread_mutex_unlock(&mutex2);

	  }
	  //Operación Get
	  else if(p.op == GET_OP){
	    // printf("%s\n", "Operation get");
	    //ejecución y rellenar respuesta
			pthread_mutex_lock(&mutex2);
			r.error = get(p.name, p.i, &(r.value));
			pthread_mutex_unlock(&mutex2);
	  }

	  //Operación destroy
	  else if(p.op == DEST_OP){
	    // printf("%s\n", "Operation destroy");
	    //ejecución y rellenar respuesta
			pthread_mutex_lock(&mutex2);
			r.error = destroy(p.name);
			pthread_mutex_unlock(&mutex2);

	  }else {
	    printf("%s\n", "Operation not found");
	  }

		/* Se devuelve el resultado al cliente */
		/* Para ello se envía el resultado a su cola */
		int qc = mq_open(p.client_queue, O_WRONLY);

		if (qc == -1){
			perror("No se puede abrir la cola del cliente");
		}
		else {
			mq_send(qc, (const char *)&r, sizeof(struct reply), NO_PRIORITY );
			mq_close(qc);
			mq_unlink(p.client_queue);
		}
	}
	pthread_exit(0);
	return NULL;
}
