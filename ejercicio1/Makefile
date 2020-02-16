BIN_FILES  = array cliente comm imple servidor

CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall

LDFLAGS = -L$(INSTALL_PATH)/lib/
LDLIBS = -lpthread -lrt


all: $(BIN_FILES)
.PHONY : all

array:
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS)

imple: imple.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(BIN_FILES) *.o
