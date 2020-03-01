#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <stdio.h>

//Métodos para que se imprima por pantalla un mensaje indicando si la operación
// ha sido correcta o incorrecta en función de lo que devuelva
void errorMsgInit(int e, int i);
void errorMsgSet(int e, int i);
void errorMsgGet(int e, int i);
void errorMsgDest(int e, int i);

int init(char *name, int N);
int set(char *name, int i, int value);
int get(char *name, int i, int *value);
int destroy(char *name);

#endif
