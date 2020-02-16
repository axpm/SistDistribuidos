#ifndef IMPLE_HPP
#define IMPLE_HPP

#include "comm.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 // Esto es para definir las cosas del server
// Poner las estructuras de datos
//Lista enlazada, problema para definir nums[size]
  //idea, al hacer el init se hace un malloc de N *sizeof(int)
struct listElement{
int *nums;
char name[MAX];
struct listElement *next;
};typedef struct listElement *List;

void insert(List *l, int *nums, char * name);
void show(List l);
void delete(List *l);


 int init(char *name, int N);

 int set(char *name, int i, int value);

 int get(char *name, int i, int *value);

 int destroy(char *name);


#endif
