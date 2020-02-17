#include "imple.h"

List server;


//Funciones para manejar la estructura de datos
void insert(List *l, int *vector, char * name){
  List p1, p2;
  p1 = *l;
  if (p1 == NULL) {
    // int sizeNew = sizeof(vector) + sizeof(struct listElement) - sizeof(int*);
    // p1 = (struct listElement *)malloc(sizeNew);
    p1 = (struct listElement *)malloc(sizeof(struct listElement));
    //datos
    *p1->vector = &vector; //Esto me va a dar error
    if (strcpy(p1->name, name) == -1)
      return -1;

    p1->next = NULL;
    *l = p1;
  }
  else{
    while (p1->next != NULL) {
      p1 = p1->next;
    }
    //NO SEGURO
    // int sizeNew = sizeof(vector) + sizeof(struct listElement) - sizeof(int*);
    // p2 = (struct listElement *)malloc(sizeNew);
    p2 = (struct listElement *)malloc(sizeof(struct listElement));
    //datos
    *p2->vector = &vector; //Esto me va a dar error
    f (strcpy(p2->name, name) == -1)
      return -1;

    p2->next = NULL;
    p1->next = p2;
  }
  return;
}


// void show(List l){
//   // while (l != NULL) {
//   //   printf("%d \n", l->num);
//   //   l = l->next;
//   // }
//   return;
// }

// void delete(List *l){
//   List aux, p;
//   p = *l;
//   if (p == NULL)
//     return;
//   else{
//     while (p != NULL) {
//       aux = p->next;
//       free(p);
//       p = aux;
//     }
//     *l = NULL;
//   }
//   return;
// }



//Funciones para manejar los datos en el server
int init(char *name, int N){
  int cod_error = 1;
  //Si se pasan argumentos erróneos, devuelve -1
  if (name == NULL || N < 0){
    fprintf(stderr, "%s\n", "Invalid arguments for the new array" );
    return -1;
  }

  //---------OJO-------------
  List aux = &server;// Pseudocódigo

  while(aux->next != NULL){
    //Si hay un elemento con el mismo nombre y distinto tamaño error

    ////NO SÉ SI ESTÁ BIEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if ( (strcmp(aux->name, name) == 0 ) && ( (int) (sizeof(*server->vector)/sizeof(int)) != N ) )
      return -1;
    if ( (strcmp(aux->name, name)  == 0) && ( (int) (sizeof(*server->vector)/sizeof(int)) == N ) )
      return 0;


    //---------OJO-------------
    aux = aux->next;// Pseudocódigo
  }

  int vector[N];
  insert(&server, &vector, name);

  return cod_error;
}

int set(char *name, int i, int value){
  //return 0;
  List aux = &server;

  if (i < 0)
    return -1;

  while (strcmp(aux->name, name) != 0) { //Compara que los nombres sean iguales
    aux = aux->next;
    if (aux == NULL){
      fprintf(stderr, "%s\n", "Not existing element");
      return -1;
    }
  }
  int N = sizeof(*aux)/sizeof(int);
  if (i >= N)
    return -1;

  //Lo ha encontrado
  for (int j = 0; j<N; j++){
    if(i == j)
      *aux[i] = value;
  }
  return 0;
}



int get(char *name, int i, int *value){
  //return 0;
  List aux = &server;

  if (i < 0)
  return -1;

  while (strcmp(aux->name, name) != 0) { //Compara que los nombres sean iguales
    aux = aux->next;
    if (aux == NULL){
      fprintf(stderr, "%s\n", "Not existing element");
      return -1;
    }
  }
  int N = sizeof(*aux)/sizeof(int);
  if (i >= N)
  return -1;

  //Lo ha encontrado
  for (int j = 0; j<N; j++){
    if(i == j)
      *value = aux[i];
  }
  return 0;
}




int destroy(char *name){
  //return 0;

  //Encontramos vector name, reasignamos punteros next y frees de vector name



}
