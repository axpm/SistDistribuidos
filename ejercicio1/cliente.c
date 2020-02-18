#include "cliente.h"

void errorMsgInit(int e, int i){
  if (e == 1)
    printf("%s %d\n", "OK; init ", i );
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

int main(int argc, char const *argv[]) {
  int init_count = 0;
  // , set_count = 0, get_count = 0, destroy_count = 0;
  // get_count += get_count + 0;

  init_count++;
  errorMsgInit( init("vector1", 100), init_count );

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



  return 0;
}
