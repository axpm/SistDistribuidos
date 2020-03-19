#include "cliente.h"

int main(int argc, char const *argv[]) {
  int init_count = 0, set_count = 0, get_count = 0, destroy_count = 0;

  printf("%s\n", "PRUEBAS CON INIT \n" );
  init_count++;
  errorMsgInit(init("vector1", 100), init_count); //Se crea el vector1 de longitud 100
  printf("%s\n", "expected: OK\n" ); //Si se está ejecutando el hilo de este cliente, debe crearlo bien
   init_count++;
   errorMsgInit(init("vector1", 100), init_count); //Se vuelve a crear el vector1 de longitud 100, devuelve 0
   printf("%s\n", "expected: Already created vector");
   init_count++;
   errorMsgInit(init("vector1", 20), init_count); //Se crea el vector1 de longitud distinta, devuelve -1
   printf("%s\n", "expected: Something went wrong" );
   init_count++;
   errorMsgInit(init("vector2", 3000), init_count); //Se crea otro vector
   printf("%s\n", "expected: OK" );
   //Esto vector3 se crea para comprobar que si borramos el vector2 (que está en medio), se borra correctamente
   init_count++;
   errorMsgInit(init("vector3",3), init_count);
   printf("%s\n", "expected: OK" );


  //Si el cliente todavía no ha ejecutado set(), el vector1 está inicializado a 0
  //Si el cliente2 ha ejecutado set(), devolverá 80
   printf("%s\n", "PRUEBAS CON GET \n" );
   int num;
   get_count ++;
   errorMsgGet(get("vector1", 1, &num), get_count); //Devuelve el valor 0 porque se ha hecho aún el set
   printf("%s %d\n", "expected: OK , num:\n", num);
   get_count++;
   errorMsgGet(get("vectorX", 1, &num), get_count); //Devuelve -1 porque el vectorX no existe
   printf("%s\n", "expected: Something went wrong \n" );
   get_count++;
   errorMsgGet(get("vector1", 120, &num), get_count); //Devuelve -1 porque la posición está fuera de rango existe
   printf("%s\n", "expected: Something went wrong \n" );

  printf("%s\n", "PRUEBAS CON SET \n" );
  set_count++;
  errorMsgSet(set("vector1", 1, 40), set_count); //Inicializamos la posición 1 con valor 40
  printf("%s\n", "expected: OK \n" );
  set_count++;
  errorMsgSet(set("vectorX", 1, 40), set_count); //Devuelve -1 porque el vectorX no existe
  printf("%s\n", "expected: Something went wrong \n" );
  set_count++;
  errorMsgSet(set("vector1", 120, 40), set_count); //Devuelve -1 porque la posición está fuera de rango existe
  printf("%s\n", "expected: Something went wrong \n" );


  printf("%s\n", "PRUEBAS CON GET \n" );
  get_count ++;
  errorMsgGet(get("vector1", 1, &num), get_count); //Devuelve valor 40 de la posición 1
  printf("%s %d\n", "expected: OK , num:\n", num);

  //Prueba con un bucle para inicializar el vector2
  //COMENTADO PARA VER MEJOR EL RESTO DE PRUEBAS
  // printf("%s\n", "PRUEBA BUCLE SET");
  // set_count = 0;
  // for (int i = 0; i < 10000; i++) {
  //   set_count++;
  //   errorMsgSet(set("vector2", i, i), set_count);
  //   printf("%s\n", "expected: OK \n" );
  // }


printf("%s\n", "PRUEBAS CON DESTROY \n" );
  destroy_count++;
  errorMsgDest(destroy("vector2"), destroy_count); //Borramos el vector 2
  printf("%s\n", "expected: OK" );
  destroy_count++;
  errorMsgDest(destroy("vector2"), destroy_count); //Se vuelve a intentar destruir el vector 2
  printf("%s\n", "expected: Something went wrong" );
  return 0;

}
void errorMsgInit(int e, int i){
  if (e == 1)
    printf("%s %d \n", "OK; init ", i );
  else if( e == 0)
      printf("%s %d\n", "Already created vector; init ", i );
  else
    printf("%s %d\n", "Something went wrong; init ", i );
}

void errorMsgSet(int e, int i) {
  if (e == 0)
    printf("%s %d\n", "OK; set ", i );
  else
    printf("%s %d\n", "Something went wrong; set ", i );
}

void errorMsgGet(int e, int i){
  if (e == 0)
    printf("%s %d\n", "OK; get ", i );
  else
    printf("%s %d\n", "Something went wrong; get ", i );
}

void errorMsgDest(int e, int i){
  if (e == 1)
    printf("%s %d\n", "OK; destroy ", i );
  else
    printf("%s %d\n", "Something went wrong; destroy ", i );
}
