#include "cliente.h"

int main(int argc, char const *argv[]) {
  int init_count = 0, set_count = 0, get_count = 0, destroy_count = 0;

  // init_count++;
  // errorMsgInit( init("vector1", 100), init_count );
  //
  // init_count++;
  // errorMsgInit( init("vector1", 10), init_count);
  //
  // init_count++;
  // errorMsgInit( init("vector2", 200), init_count);
  //
  // init_count++;
  // errorMsgInit( init("vector2", 200), init_count);
  //
  // set_count++;
  // errorMsgSet( set("vector1", 0, 40), set_count);
  //
  // set_count++;
  // errorMsgSet( set("vector1", 120, 30), set_count);
  //
  // init_count++;
  // errorMsgInit( init("vector1", 200), init_count );
  //
  // destroy_count++;
  // errorMsgDest( destroy("vector1") , destroy_count);
  //
  // destroy_count++;
  // errorMsgDest( destroy("vector") , destroy_count);


  printf("%s\n", "PRUEBAS CON INIT \n" );
  init_count++;
  errorMsgInit(init("vector1", 10), init_count);
  printf("%s\n", "expected: OK\n" );
   init_count++;
   errorMsgInit(init("vector1", 10), init_count);
   printf("%s\n", "expected: Already created vector");
   init_count++;
   errorMsgInit(init("vector1", 20), init_count);
   printf("%s\n", "expected: Something went wrong" );
   init_count++;
   errorMsgInit(init("vector2", 5), init_count);
   printf("%s\n", "expected: OK" );



  printf("%s\n", "PRUEBAS CON SET \n" );
  set_count++;
  errorMsgSet(set("vector1", 1, 40), set_count);
  printf("%s\n", "expected: OK \n" );
  set_count++;
  errorMsgSet(set("vector1", 12, 40), set_count);
  printf("%s\n", "expected: Something went wrong \n" );
  set_count++;
  errorMsgSet(set("vectorX", 1, 40), set_count);
  printf("%s\n", "expected: Something went wrong \n" );



  printf("%s\n", "PRUEBAS CON GET \n" );
  int num;
  get_count ++;
  errorMsgGet(get("vector1", 1, &num), get_count);
  printf("%s \n", "expected: OK \n");
  get_count++;
  errorMsgGet(get("vectorX", 1, &num), get_count);
  printf("%s\n", "expected: Something went wrong \n" );
  get_count++;
  errorMsgGet(get("vector1", 12, &num), get_count);
  printf("%s\n", "expected: Something went wrong \n" );
  // destroy_count++;
  // errorMsgDest(destroy("vector1"), destroy_count);
  // printf("%s\n", "expected: OK" );
  // destroy_count++;
  // errorMsgDest(destroy("vector"), destroy_count);
  // printf("%s\n", "expected: Something went wrong");
  // init_count++;
  // errorMsgInit(init("vector1", 2), init_count);
  // printf("%s\n", "expected: OK" );
  // init_count++;
  // errorMsgInit(init("vector3",3), init_count);
  // printf("%s\n", "expected: OK" );
  // init_count++;
  // errorMsgInit(init("vector4", 5), init_count);
  // printf("%s\n", "expected: OK" );
  // init_count++;
  // errorMsgInit(init("vector5", 2), init_count);
  // printf("%s\n", "expected: OK" );

printf("%s\n", "PRUEBAS CON DESTROY \n" );
  destroy_count++;
  errorMsgDest(destroy("vector1"), destroy_count);
  printf("%s\n", "expected: OK" );
  destroy_count++;
  errorMsgDest(destroy("vector1"), destroy_count);
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
