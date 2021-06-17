CC=g++
CFLAGS=-g -I.
DEPS = Socket.h Chat.h Server.h Client.h
OBJ = Socket.o Chat.o Server.o Client.o
LIBS=-lpthread

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: serverApp clientApp

serverApp: $(OBJ) MainServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

clientApp: $(OBJ) MainClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o serverApp clientApp
