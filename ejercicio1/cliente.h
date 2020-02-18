#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <stdio.h>

void errorMsgInit(int e, int i);
void errorMsgSet(int e, int i);
void errorMsgGet(int e, int i);
void errorMsgDest(int e, int i);


//NO MUY CLARO QUE ESTO TENGA QUE IR AQUÍ
int init(char *name, int N);

int set(char *name, int i, int value);

int get(char *name, int i, int *value);

int destroy(char *name);


#endif
