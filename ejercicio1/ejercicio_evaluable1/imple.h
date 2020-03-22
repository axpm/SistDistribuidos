#ifndef IMPLE_HPP
#define IMPLE_HPP

#include "comm.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Estructura de datos formado por el nombre del vector, el tamaño,
//puntero al vector y puntero al siguiente elemento de la lista
struct listElement{
 char name[MAX];
 int N;
 int *vector;
struct listElement *next;
};typedef struct listElement *List;

void show(List l);
int init(char *name, int N);
int set(char *name, int i, int value);
int get(char *name, int i, int *value);
int destroy(char *name);

#endif
