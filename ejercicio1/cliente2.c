#include "cliente.h"
#include <unistd.h>

int main(int argc, char const *argv[]) {
  int init_count = 0, set_count = 0, destroy_count = 0;
  //get_count = 0

//--------------------------PRUEBA 1 CONCURRENCIA -------------------------------------------------------
  // printf("%s\n", "PRUEBAS CON INIT:Creamos el vector 1" );
  // init_count++;
  // //Si el otro cliente ha creado el vector1 de longitud 100, devolverá que ya existe,
  // //si ejecuta este primero, lo hará correctamente
  // errorMsgInit(init("vector1", 1000), init_count);
  // printf("%s\n", "" );
  //
  //   //Prueba con un bucle para inicializar el vector 1
  //   printf("%s\n", "RELLENAMOS CON UN BUCLE DE 1000 SET EL VECTOR1...");
  //   set_count = 0;
  //   for (int i = 0; i < 1000; i++) {
  //     set_count++;
  //     set("vector1", i, i);
  //
  //   }
  //
  // printf("%s\n", "" );
  // printf("%s\n", "DESTROY:Borramos el vector 2 que creó el otro cliente" );
  // destroy_count++;
  // errorMsgDest(destroy("vector2"), destroy_count); //Borramos el vector 2 que creó el cliente primero
  // printf("%s\n", "expected: OK" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );
  // printf("%s\n", "" );


  //---------------------------------------- FIN PRUEBA 1 -------------------------------------------
  //----------------------------------------PRUEBA 2 CONCURRENCIA ----------------------------------------

printf("%s\n", "Creamos el vector 2" );
init_count++;
errorMsgInit(init("vector2", 3000), init_count); //Se crea otro vector
printf("%s\n", "expected: OK" );
printf("%s\n", "Se duerme" );
sleep(2);
printf("%s\n", "Se despierta" );
printf("%s\n", "Volvemos a borrar el vector 2" );
destroy_count++;
errorMsgDest(destroy("vector2"), destroy_count); //Borramos el vector 2 que creó el cliente primero
printf("%s\n", "expected: OK" );
printf("%s\n", "" );
printf("%s\n", "" );
printf("%s\n", "" );
printf("%s\n", "" );
printf("%s\n", "" );
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
