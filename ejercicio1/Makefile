BIN_FILES  = array imple cliente servidor

CC = gcc

LD = ld

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lpthread


all: $(BIN_FILES)
.PHONY : all

array:
	$(CC) -Wall -fpic -c array.c -o $@.o
	$(LD) -o libarray.so $@.o -shared

cliente: cliente.o
	gcc -Wall -o $@ cliente.o -L. -larray -lrt
	chmod +x script.sh
	./script.sh

servidor: servidor.o imple.o
	gcc -Wall -o $@ servidor.o -lrt -lpthread


clean:
	rm -f $(BIN_FILES) *.o *.so
