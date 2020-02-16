#include "imple.h"

List server;


//Funciones para manejar la estructura de datos
void insert(List *l, int *nums, char * name){
  List p1, p2;
  p1 = *l;
  if (p1 == NULL) {
    int sizeNew = sizeof(nums) + sizeof(struct listElement) - sizeof(struct List*);
    p1 = (struct listElement *)malloc(sizeof(sizeNew));
    // p1 = (struct List *)malloc(sizeof(struct List));
    //datos
    p1->nums = nums; //Esto me va a dar error
    strcpy(p1->name, name);
    p1->next = NULL;
    *l = p1;
  }
  else{
    while (p1->next != NULL) {
      p1 = p1->next;
    }
    //NO SEGURO
    int sizeNew = sizeof(nums) + sizeof(struct listElement) - sizeof(struct List*);
    //El tamaño del nuevo elemento, es el tamaño del array nuevo más todo el struct, menos el puntero
    //que luego es un array
    p2 = (struct listElement *)malloc(sizeof(sizeNew));
    // p2 = (struct List *)malloc(sizeof(struct List));
    //datos
    p2->nums = nums; //Esto me va a dar error
    p2->next = NULL;
    p1->next = p2;
  }
  return;
}


void show(List l){
  // while (l != NULL) {
  //   printf("%d \n", l->num);
  //   l = l->next;
  // }
  return;
}

void delete(List *l){
  List aux, p;
  p = *l;
  if (p == NULL)
    return;
  else{
    while (p != NULL) {
      aux = p->next;
      free(p);
      p = aux;
    }
    *l = NULL;
  }
  return;
}



//Funciones para manejar los datos en el server
int init(char *name, int N){
  int cod_error = 1;
  //Si se pasan argumentos erróneos, devuelve -1
  if (name == NULL || N < 0){
    fprintf(stderr, "%s\n", "Invalid arguments for the new array" );
    return -1;
  }
  List aux = server;
  while(aux == NULL){
    //Si hay un elemento con el mismo nombre y distinto tamaño error

    ////NO SÉ SI ESTÁ BIEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if ( (strcmp(aux->name, name) == 0 ) && ( (int) (sizeof(*server->nums)/sizeof(int)) != N ) )
      return -1;
    if ( (strcmp(aux->name, name)  == 0) && ( (int) (sizeof(*server->nums)/sizeof(int)) == N ) )
    //////////////////////////////////ESTO ----------------------------------------
      cod_error = 0;
  }

  int nums[N];
  insert(&server, nums, name);

  return cod_error;
}

int set(char *name, int i, int value){
  return 0;
}

int get(char *name, int i, int *value){
  return 0;
}

int destroy(char *name){
  return 0;
}
