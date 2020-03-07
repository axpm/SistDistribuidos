#include "imple.h"

//Variable global que representa la lista que contiene los vectores
List server;

//INIT
int init(char *name, int N){
  int cod_error = 1;
  //Si se pasan argumentos erróneos (que no tenga nombre o que la dimenión sea negativa o 0), devuelve -1
  if (name == NULL || N <= 0){
    fprintf(stderr, "%s\n", "Invalid arguments for the new array" );
    return -1;
  }

  List aux = server;
  List prev = server;
  //En el caso de que esté vacía la lista
  if (server == NULL){
    List newElement = (struct listElement *)malloc(sizeof(struct listElement));//Creamos un nuevo elemento
    strcpy(newElement->name, name);//Le pasamos el nombre
    newElement->N=N; //Le pasamos el tamaño
    newElement->vector = (int *)malloc(N*sizeof(int));//Creamos el vector
    newElement->next = NULL;//Hacemos que el puntero al siguiente elemento apunte a NULL
    server = newElement;//El puntero a la lista apunta al nuevo elemento
    return cod_error;

  }
  //Si no está vacía recorremos al lista buscando el nombre
  while(aux != NULL){
    if ( (strcmp(aux->name, name) == 0) && aux->N == N ){ //Si ya existe el vector con el mismo tamaño
      return 0;
    }
    if ( (strcmp(aux->name, name) == 0) && aux->N != N ){ //Si ya existe el vector con tamaño distinto
      return -1;
    }
    prev=aux;//Guardamos el anterior
    aux = aux->next;//Hacemos avanzar los elementos de la lista
  }
  //Creamos un nuevo elemento al que le pasaremos el nombre, el tamaño y el vector. El puntero al siguiente apunatrá a NULL
  List newElement = (struct listElement *)malloc(sizeof(struct listElement));
  if (newElement == NULL){
    perror("malloc");
    return -1;
  }
  strcpy(newElement->name, name);
  newElement->N=N;
  newElement->vector = (int *)malloc(N*sizeof(int));
  if ( newElement->vector == NULL){
    perror("malloc");
    return -1;
  }
  newElement->next = NULL;
  prev->next = newElement;
  return cod_error;
}


//SET
int set(char *name, int i, int value){
  List aux = server;
  //Tratamos los errores de lista vacía, posición negativa o mayor que el tamaño del vector
  if (server == NULL || i < 0 || i >=aux->N)
  return -1;

  while (strcmp(aux->name, name) != 0) { //Compara que los nombres sean iguales
    aux = aux->next;
    if (aux == NULL){//Si se acaba la lista y no ha encontrado el nombre
      fprintf(stderr, "%s\n", "Not existing element");
      return -1;
    }
  }

  //Si encontramos el nombre del vector, cambiamos el valor de la posición i
  aux->vector[i] = value;
  return 0;
}

//GET
int get(char *name, int i, int *value){
  List aux = server;

  //Tratamos los errores de lista vacía, posición negativa o mayor que el tamaño del vector
  if (server == NULL || i < 0 || i >=aux->N)
  return -1;

  while (strcmp(aux->name, name) != 0) { //Compara que los nombres sean iguales
    aux = aux->next;
    if (aux == NULL){//Si se acaba la lista y no ha encontrado el nombre
      fprintf(stderr, "%s\n", "Not existing element");
      return -1;
    }
  }
  //Si encontramos el nombre del vector, guardamos el valor de la posición i
  *value = aux->vector[i];

  return 0;
}

//DESTROY
int destroy(char *name){
//Si la lista está vacía, error
  if (server == NULL)
    return -1;

  List aux = server;
  List prev = server; //Necesitamos un previo para hacer el borrado

  //Si es el primero
  if (strcmp(aux->name, name) == 0) {
    server = aux->next;
    free(aux);
  }

  else { //Si no lo es
    while (strcmp(aux->name, name) != 0) { //Compara que los nombres sean iguales
      if (aux->next == NULL){
        fprintf(stderr, "%s\n", "Not existing element");
        return -1;
      }
      prev = aux;
      aux = aux->next;
    }
    prev->next = aux->next;
    free(aux); //Hacemos free de la posición actual
  }
  return 1;
}


//SHOW añadido para ver la lista en la batería de pruebas
void show (List l){
  while (l != NULL){
    printf("%s\n", l->name);
    for (int i = 0; i<l->N; i++){
      printf("%d\n", l->vector[i]);
    }
    l = l->next;
  }
  return;
}
