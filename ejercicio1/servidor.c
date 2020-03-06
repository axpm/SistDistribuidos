#include "servidor.h"
#include <signal.h>

pthread_mutex_t mutex;
pthread_cond_t c;
bool copiado; //variable de condición

//salir con signal
bool fin = false;

void sigint_handler(int sig) {
	// fin = true;
	mq_unlink(NOMBRE_SERVER);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&c);
	exit(0);
	// printf("hola\n");
}

int main(int argc, char const *argv[]) {
  //Hilos
	pthread_attr_t attrTh;
  pthread_t t;
  pthread_attr_init(&attrTh);
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&c, NULL);
  copiado = false;

	//Manejador de la señal
	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);

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


  while(!fin){

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
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&c);
  return 0;
}

void listenPetition(struct petition * pet){

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
    perror("mq_open");
    pthread_exit(NULL);
  }

  //OPERACIONES
  //Operación Init
  if(p.op == INIT_OP){
    printf("%s\n", "Operation init");
    //ejecución y rellenar respuesta
    r.error = init(p.name, p.N);

  }
  //Operación Set
  else if(p.op == SET_OP){
    printf("%s\n", "Operation set");
    //ejecución y rellenar respuesta
   r.error = set(p.name, p.i, p.value);

  }
  //Operación Get
  else if(p.op == GET_OP){
    printf("%s\n", "Operation get");
    //ejecución y rellenar respuesta
    r.error = get(p.name, p.i, &(r.value));
  }

  //Operación destroy
  else if(p.op == DEST_OP){
    printf("%s\n", "Operation destroy");

    //ejecución y rellenar respuesta
    r.error = destroy(p.name);

  }else {
    printf("%s\n", "Operation not found");
  }


  //Enviar respuesta
  int e = mq_send(qc, (const char *)&r, sizeof(struct reply), NO_PRIORITY );
  if (e == -1){
    perror("mq_send");
		mq_close(qc);
    mq_unlink(NOMBRE_SERVER);
    mq_unlink(p.client_queue);
    pthread_exit(NULL);
  }

	mq_close(qc);
  mq_unlink(p.client_queue);
	pthread_exit(NULL);

}
