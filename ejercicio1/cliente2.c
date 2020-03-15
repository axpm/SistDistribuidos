#include "cliente.h"

int main(int argc, char const *argv[]) {
  int init_count = 0, set_count = 0, get_count = 0, destroy_count = 0;

  printf("%s\n", "PRUEBAS CON INIT \n" );
  init_count++;
  //Si el otro cliente ha creado el vector1 de longitud 100, devolverá que ya existe,
  //si ejecuta este primero, lo hará correctamente
  errorMsgInit(init("vector1", 100), init_count);
  //Si ningún cliente todavía no ha ejecutado set(), en el vector1 en la posición 1, debe devolver 0
  //Si el otro cliente va primero, ha ejecutado set() y devolverá 40
  //Si el cliente2 va después, ha ejecutado set(), devolverá 0
   printf("%s\n", "PRUEBAS CON GET \n" );
   int num;
   get_count ++;
   errorMsgGet(get("vector1", 1, &num), get_count); //Si se ejecuta primero devuelve 0, si no es así, 40
   printf("%s %d\n", "expected: OK , num:\n", num);

  printf("%s\n", "PRUEBAS CON SET \n" );
  set_count++;
  errorMsgSet(set("vector1", 1, 80), set_count); //Escribe 80 en la posición 1


  printf("%s\n", "PRUEBAS CON GET \n" );
  get_count ++;
  errorMsgGet(get("vector1", 1, &num), get_count); //Devuelve 80 
  printf("%s %d\n", "expected: OK , num:\n", num);


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
