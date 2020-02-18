BIN_FILES  = array cliente comm imple servidor

CC = gcc

LD = ld

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lpthread


all: $(BIN_FILES)
.PHONY : all

array:
	$(CC) -Wall -fPIC -c array.c -o $@.o
	$(LD) -o libarray.so $@.o -shared

imple: imple.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

cliente: cliente
	gcc -Wall -o $@ cliente.c -L. –larray
	export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

clean:
	rm -f $(BIN_FILES) *.o *.so
