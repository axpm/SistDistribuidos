#include "cliente.h"
#include <unistd.h>

int main(int argc, char const *argv[]) {
  int init_count = 0,  get_count = 0, destroy_count = 0;

  //  set_count = 0,

//--------------------------PRUEBA 1 CONCURRENCIA -------------------------------------------------------
  // printf("%s\n", "PRUEBAS CON INIT: Creamos el vector 1" );
  // init_count++;
  // errorMsgInit(init("vector1", 1000), init_count); //Se crea el vector1 de longitud 100
  // printf("%s\n", "expected: OK\n" ); //Si se está ejecutando el hilo de este cliente, debe crearlo bien
  //   printf("%s\n", "Creamos el vector 2" );
  // init_count++;
  //  errorMsgInit(init("vector2", 3000), init_count); //Se crea otro vector
  //  printf("%s\n", "expected: OK" );
  //    printf("%s\n", "Creamos el vector 3" );
  //  //Esto vector3 se crea para comprobar que si borramos el vector2 (que está en medio), se borra correctamente
  //  init_count++;
  //  errorMsgInit(init("vector3",3), init_count);
  //  printf("%s\n", "expected: OK" );
  //
  // printf("%s\n", "" );
  //
  // //Si el cliente todavía no ha ejecutado set(), el vector1 está inicializado a 0
  // //Si el cliente2 ha ejecutado set(), devolverá 80
  //  printf("%s\n", "PRUEBAS CON GET: Leemos la posición 80" );
  //  int num;
  //  get_count ++;
  //  errorMsgGet(get("vector1", 80, &num), get_count); //Devuelve el valor 0 porque se ha hecho aún el set
  //  printf("%s %d\n", "expected: OK , num:\n", num);
  //  printf("%s\n", "Se duerme" );
  //  sleep(2);
  //  printf("%s\n", "Se despierta" );
  //
  // printf("%s\n", "" );
  // printf("%s\n", "PRUEBAS CON GET: Volvemos a leer la posición 80" );
  // get_count ++;
  // errorMsgGet(get("vector1", 80, &num), get_count); //Devuelve valor 40 de la posición 1
  // printf("%s %d\n", "expected: OK , num:\n", num);

//---------------------------------------- FIN PRUEBA 1 -------------------------------------------
//----------------------------------------PRUEBA 2 CONCURRENCIA ----------------------------------------
printf("%s\n", "PRUEBAS CON INIT: Creamos el vector 1" );
init_count++;
errorMsgInit(init("vector1", 1000), init_count); //Se crea el vector1 de longitud 100
printf("%s\n", "expected: OK" ); //Si se está ejecutando el hilo de este cliente, debe crearlo bien
printf("%s\n", "Se duerme" );
sleep(2);
printf("%s\n", "Se despierta" );
printf("%s\n", "Creamos el vector 3" );
//Esto vector3 se crea para comprobar que si borramos el vector2 (que está en medio), se borra correctamente
init_count++;
errorMsgInit(init("vector3",3), init_count);
printf("%s\n", "expected: OK" );
printf("%s\n", "Borramos el vector 2" );
destroy_count++;
errorMsgDest(destroy("vector2"), destroy_count); //Borramos el vector 2 que creó el cliente primero
printf("%s\n", "expected: OK" );

//---------------------------------------- FIN PRUEBA 2 -------------------------------------------
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
