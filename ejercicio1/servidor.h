#ifndef SERVIDOR_HPP
#define SERVIDOR_HPP

#define MAX_THREADS 16

#include "imple.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <signal.h>

void listenPetition(struct petition * pet);

void* servicio(void);

#endif
