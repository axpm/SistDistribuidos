#include "imple.h"

List server;

//PRUEBAS HECHAS EN MODO LOCAL
// int main(int argc, char const *argv[]) {
//   int a =init("vector1", 10);
//   printf("%s %d\n", "Init Vector1", a );
//   printf("%s\n","mostramos la lista añadiendo vector1 " );
//   show(server);
//
//   int b =init("vector1", 10);
//   printf("%s %d\n", "Init2 Vector1", b );
//   printf("%s\n","mostramos la lista sin crear de nuevo vector1 " );
//   show(server);
//
//   int c =init("vector1", 20);
//   printf("%s %d\n", "Init2 Vector1", c );
//   printf("%s\n","mostramos la lista " );
//   show(server);
//
//   int d =init("vector2", 5);
//    printf("%s %d\n", "Init Vector2", d );
//   printf("%s\n","mostramos la lista creando vector2" );
//   show(server);
//
//   int e = set("vector1", 1, 40);
//    printf("%s %d\n", "Set Vector1", e );
//   printf("%s\n","mostramos la lista despues de set en vector 1" );
//   show(server);
//
//   int num;
//   int f =get("vector1", 1, &num);
//   printf("%s %d \n","salida del get ",num );
//   printf("%s %d\n", "salida del get ", f);
//
//   int g = destroy("vector1");
//   printf("%s %d\n", "destroy Vector1", g );
//   show(server);
//
//   int h = destroy("vector");
//   printf("%s %d\n", "destroy Vector", h );
//   show(server);
//
//   int i = init("vector1", 2);
//   printf("%s %d\n", "Init Vector1", i );
// show(server);
//
//
//    i = init("vector3",3);
//   printf("%s %d\n", "Init Vector3", i );
// show(server);
//
//    i = init("vector4", 5);
//   printf("%s %d\n", "Init Vector4", i );
// show(server);
//
//  i = init("vector5", 2);
// printf("%s %d\n", "Init Vector5", i );
// show(server);
//
//
// g = destroy("vector1");
//   printf("%s %d\n", "destroy Vector1", g );
//   show(server);
//
//
//      g = destroy("vector5");
//     printf("%s %d\n", "destroy Vector5", g );
//     show(server);
// }

int init(char *name, int N){
  int cod_error = 1;
  //Si se pasan argumentos erróneos, devuelve -1
  if (name == NULL || N <= 0){
    fprintf(stderr, "%s\n", "Invalid arguments for the new array" );
    return -1;
  }

  List aux = server;
  List prev = server;
  //mirar que este vacía
  if (server == NULL){
    List newElement = (struct listElement *)malloc(sizeof(struct listElement));
    strcpy(newElement->name, name);
    newElement->N=N;
    newElement->vector = (int *)malloc(N*sizeof(int));
    newElement->next = NULL;
    server = newElement;
    return cod_error;

  }
  //no vacía
  while(aux != NULL){
    //    if ( (strcmp(aux->name, name) == 0) && ( (int) (sizeof(aux->vector)/sizeof(int)) == N ) ){
    if ( (strcmp(aux->name, name) == 0) && aux->N == N ){
      return 0;
    }
    //    if ( (strcmp(aux->name, name) == 0 ) && ( (int) (sizeof(aux->vector)/sizeof(int)) != N ) ){
    if ( (strcmp(aux->name, name) == 0) && aux->N != N ){
      return -1;
    }
    prev=aux;
    aux = aux->next;
  }
  List newElement = (struct listElement *)malloc(sizeof(struct listElement));
  strcpy(newElement->name, name);
  newElement->N=N;
  newElement->vector = (int *)malloc(N*sizeof(int));
  newElement->next = NULL;
  prev->next = newElement;
  return cod_error;
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


//SET
int set(char *name, int i, int value){
  List aux = server;
  if (server == NULL || i < 0 || i >=aux->N)
  return -1;

  while (strcmp(aux->name, name) != 0) { //Compara que los nombres sean iguales
    aux = aux->next;
    if (aux == NULL){
      fprintf(stderr, "%s\n", "Not existing element");
      return -1;
    }
  }
  // int N = sizeof(*aux)/sizeof(int);
  // if (i >= N)
  //   return -1;

  aux->vector[i] = value;
  //}
  return 0;
}

//GET
int get(char *name, int i, int *value){
  //return 0;
  List aux = server;

  if (server == NULL || i < 0 || i >=aux->N)
  return -1;

  while (strcmp(aux->name, name) != 0) { //Compara que los nombres sean iguales
    aux = aux->next;
    if (aux == NULL){
      fprintf(stderr, "%s\n", "Not existing element");
      return -1;
    }
  }
  *value = aux->vector[i];

  return 0;
}

//DESTROY
int destroy(char *name){
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




// #include "imple.h"
//
// List server;
//
//
// //Funciones para manejar la estructura de datos
// void insert(List *l, int *vector, char * name){
//   List p1, p2;
//   p1 = *l;
//   if (p1 == NULL) {
//     // int sizeNew = sizeof(vector) + sizeof(struct listElement) - sizeof(int*);
//     // p1 = (struct listElement *)malloc(sizeNew);
//     p1 = (struct listElement *)malloc(sizeof(struct listElement));
//     //datos
//     p1->vector = vector; //Esto me va a dar error
//     strcpy(p1->name, name);
//
//     p1->next = NULL;
//     *l = p1;
//   }
//   else{
//     while (p1->next != NULL) {
//       p1 = p1->next;
//     }
//     //NO SEGURO
//     // int sizeNew = sizeof(vector) + sizeof(struct listElement) - sizeof(int*);
//     // p2 = (struct listElement *)malloc(sizeNew);
//     p2 = (struct listElement *)malloc(sizeof(struct listElement));
//     //datos
//     p2->vector = vector; //Esto me va a dar error
//     strcpy(p1->name, name);
//
//
//     p2->next = NULL;
//     p1->next = p2;
//   }
//   return;
// }
//
//
// // void show(List l){
// //   // while (l != NULL) {
// //   //   printf("%d \n", l->num);
// //   //   l = l->next;
// //   // }
// //   return;
// // }
//
// // void delete(List *l){
// //   List aux, p;
// //   p = *l;
// //   if (p == NULL)
// //     return;
// //   else{
// //     while (p != NULL) {
// //       aux = p->next;
// //       free(p);
// //       p = aux;
// //     }
// //     *l = NULL;
// //   }
// //   return;
// // }
//
//
//
// //Funciones para manejar los datos en el server
// int init(char *name, int N){
//   int cod_error = 1;
//   //Si se pasan argumentos erróneos, devuelve -1
//   if (name == NULL || N <= 0){
//     fprintf(stderr, "%s\n", "Invalid arguments for the new array" );
//     return -1;
//   }
//
//   //---------OJO-------------
//   List aux = server;// Pseudocódigo
//   //mirar que este vacía arriba
//   if (server == NULL){
//     List newElement = (struct listElement *)malloc(sizeof(struct listElement));
//     strcpy(newElement->name, name);
//     int vector[N];
//     newElement->vector = vector;
//     newElement->next = NULL;
//     server = newElement;
//   }
//
//   while(aux->next != NULL){
//     //Si hay un elemento con el mismo nombre y distinto tamaño error
//
//     ////NO SÉ SI ESTÁ BIEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//     if ( (strcmp(aux->name, name) == 0 ) && ( (int) (sizeof(*server->vector)/sizeof(int)) != N ) )
//       return -1;
//     if ( (strcmp(aux->name, name) == 0) && ( (int) (sizeof(*server->vector)/sizeof(int)) == N ) )
//       return 0;
//     //---------OJO-------------
//     aux = aux->next;// Pseudocódigo
//   }
//
//   //int vector[N];
//   //insert(&server, vector, name);
//   //EN vez de llamar al insert
//   List newElement = (struct listElement *)malloc(sizeof(struct listElement));
//   strcpy(newElement->name, name);
//   int vector[N];
//   newElement->vector = vector;
//   newElement->next = NULL;
//   server = newElement;
//   return cod_error;
// }
//
// int set(char *name, int i, int value){
//   //return 0;
//   List aux = server;
//   if (server == NULL || i < 0)
//     return -1;
//
//   while (strcmp(aux->name, name) != 0) { //Compara que los nombres sean iguales
//     aux = aux->next;
//     if (aux == NULL){
//       fprintf(stderr, "%s\n", "Not existing element");
//       return -1;
//     }
//   }
//   int N = sizeof(*aux)/sizeof(int);
//   if (i >= N)
//     return -1;
//
//   //Lo ha encontrado
//   //for (int j = 0; j<N; j++){
//     //if(i == j)
//     //Sobraría porque vamos a una posición concreta
//       aux->vector[i] = value;
//   //}
//   return 0;
// }
//
//
//
// int get(char *name, int i, int *value){
//   //return 0;
//   List aux = server;
//
//   if (server == NULL || i < 0)
//     return -1;
//
//   while (strcmp(aux->name, name) != 0) { //Compara que los nombres sean iguales
//     aux = aux->next;
//     if (aux == NULL){
//       fprintf(stderr, "%s\n", "Not existing element");
//       return -1;
//     }
//   }
//   int N = sizeof(*aux)/sizeof(int);
//   if (i >= N)
//   return -1;
//
//   //Lo ha encontrado
// //for (int j = 0; j<N; j++){
//   //  if(i == j)
//       *value = aux->vector[i];
//   //}
//   return 0;
// }
//
// // int destroy(char *name){
// //   // return 0;
// //   //Encontramos vector name, reasignamos punteros next y frees de vector name
// //
// //   List aux = server;
// // while (strcmp(aux->next->name, name) != 0) { //Compara que los nombres sean iguales
// //   aux = aux->next;
// //   if (aux->next == NULL){
// //     fprintf(stderr, "%s\n", "Not existing element");
// //     return -1;
// //   }
// // }
// // // El que queremos eliminar es aux->next, por tanto queremos que aux apunte al next del next
// // aux->next = aux->next->next;
// // aux = aux->next;
// // free(aux); //Hacemos free de la posición actual
// //
// // return 1;
// //}
//
// int destroy(char *name){
//   // return 0;
//   if (server == NULL)
//     return -1;
//
//   List aux = server;
//   List prev = server; //Necesitamos un previo para hacer el borrado
//   //Si es el primero
//   if (strcmp(aux->name, name) != 0) {
//     server = aux->next;
//     free(aux);
//   }
//   else { //Si no lo es
//     while (strcmp(aux->name, name) != 0) { //Compara que los nombres sean iguales
//       prev = aux;
//       aux = aux->next;
//       if (aux->next == NULL){
//         fprintf(stderr, "%s\n", "Not existing element");
//         return -1;
//       }
//     }
//     prev->next = aux->next;
//     free(aux); //Hacemos free de la posición actual
//   }
//   return 1;
// }
