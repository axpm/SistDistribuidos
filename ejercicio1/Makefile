BIN_FILES  = array cliente cliente2 servidor servidorPool

CC = gcc

LD = ld

CPPFLAGS = -I$(INSTALL_PATH)/include
CFLAGS =	-Wall  -g

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lpthread -lrt


all: $(BIN_FILES)
.PHONY : all

array: array.o
	$(CC) -Wall -fpic -c array.c -o $@.o
	$(LD) -o libarray.so $@.o -shared

cliente: cliente.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -L. -larray -lrt

cliente2: cliente2.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -L. -larray -lrt

servidor: servidor.o imple.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

servidorPool: servidorPool.o imple.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f $(BIN_FILES) *.o *.so
