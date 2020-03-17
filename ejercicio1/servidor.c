#include "servidor.h"
#include <signal.h>

pthread_mutex_t mutex;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;
pthread_cond_t c;
pthread_cond_t c2;
bool copiado; //variable de condición

int qs = -1; //cola del server
int nt = 0; //number of threads

void sigint_handler(int sig) {

	//compureba el estado del contador de hilos
	pthread_mutex_lock(&mutex3);
	while(nt > 0)
		pthread_cond_wait(&c2, &mutex3);
	pthread_mutex_unlock(&mutex3);

	//si han acabado todos los hilos ya acaba
	mq_close(qs);
	mq_unlink(NOMBRE_SERVER);
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex2);
	pthread_mutex_destroy(&mutex3);
	pthread_cond_destroy(&c);
	pthread_cond_destroy(&c2);
	printf("%s\n", "\nServer Closed" );
	exit(0);
}

int main(int argc, char const *argv[]) {
  //Hilos
	pthread_attr_t attrTh;
  pthread_t t;
  pthread_attr_init(&attrTh);
	pthread_attr_setdetachstate(&attrTh,PTHREAD_CREATE_DETACHED);
  pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_mutex_init(&mutex3, NULL);
	pthread_cond_init(&c, NULL);
	pthread_cond_init(&c2, NULL);
  copiado = false;

	//Manejador de la señal ctrl-c
	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);

	//Atributos
	struct mq_attr attr;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = sizeof(struct petition);

  //Cola servidor
	qs = mq_open(NOMBRE_SERVER, O_CREAT | O_RDWR, 0777, &attr);
  if (qs == -1){
    perror("mq_open");
    fprintf(stderr, "%s\n", "Error! Couldn't create Server Queue");
    exit(-1);
  }


  while(1){

    //CREAR HILOS BAJO DEMANDA
		struct petition p;
		int e = mq_receive(qs, (char *)&p, sizeof(struct petition), NO_PRIORITY );
		if (e == -1){
			perror("mq_receive");
			exit(0);
		}
    if (pthread_create(&t, NULL, (void *) listenPetition, &p) == -1){
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

	mq_close(qs);
	mq_unlink(NOMBRE_SERVER);
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex2);
	pthread_mutex_destroy(&mutex3);
	pthread_cond_destroy(&c);
	pthread_cond_destroy(&c2);
  return 0;
}

void listenPetition(struct petition * pet){

	pthread_mutex_lock(&mutex3);
  nt ++;
	printf("%d\n", nt );
  pthread_mutex_unlock(&mutex3);

  //Petición
  struct petition p =  *pet;

  //se ha recibido y copiado en petición
  pthread_mutex_lock(&mutex);
  copiado = true;
  pthread_cond_signal(&c);
  pthread_mutex_unlock(&mutex);
	
  //Respuesta
  struct reply r;

  //Cola cliente
  int qc = mq_open(p.client_queue, O_WRONLY);

  if (qc == -1){
		printf("%s\n", p.client_queue);
    perror("mq_open: error in opening client_queue on sending the reply");
    pthread_exit(NULL);
  }

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


  //Enviar respuesta
  int e = mq_send(qc, (const char *)&r, sizeof(struct reply), NO_PRIORITY );
  if (e == -1){
    perror("mq_send");
		mq_close(qc);
    mq_unlink(NOMBRE_SERVER);
    pthread_exit(NULL);
  }

	mq_close(qc);
	pthread_mutex_lock(&mutex3);
	nt--;
	printf("%d\n", nt );
	pthread_cond_signal(&c2);
	pthread_mutex_unlock(&mutex3);

	pthread_exit(NULL);

}
