BIN_FILES  = rpcServer server

CC = gcc


CCGLAGS =	-Wall  -g

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lpthread


all: CFLAGS=$(CCGLAGS)
all: $(BIN_FILES)
.PHONY : all

server: server.o lines.o imple.o storage_xdr.o storage_clnt.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

rpcServer: storage_xdr.o storage_svc.o imple.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) servidorAlmacenamiento.c -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f $(BIN_FILES) *.o

.SUFFIXES:
.PHONY : clean
