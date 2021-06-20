CC=g++
CFLAGS=-g -I.
DEPS = Socket.h Message.h Server.h Client.h
OBJ = Socket.o Message.o Server.o Client.o
LIBS=-lpthread -lSDL2 -lSDL2_image

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: QuienEsQuienApp

QuienEsQuienApp: $(OBJ) main.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o QuienEsQuienApp
