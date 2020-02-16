#ifndef ARRAY_HPP
#define  ARRAY_HPP

#include "comm.h"
#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int init(char *name, int N);

int set(char *name, int i, int value);

int get(char *name, int i, int *value);

int destroy(char *name);


#endif
