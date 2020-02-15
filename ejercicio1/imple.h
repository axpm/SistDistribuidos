#ifndef IMPLE_HPP
#define IMPLE_HPP

 // Esto es para definir las cosas del server
// Poner las estructuras de datos
 int init(char *name, int N);

 int set(char *name, int i, int value);

 int get(char *name, int i, int *value);

 int destroy(chat *name);


#endif
