CC=g++
CFLAGS=-g -I.
DEPS = Socket.h Message.h Server.h Client.h ButtonPermanent.h Button.h
OBJ = Socket.o Message.o Server.o Client.o ButtonPermanent.o Button.o
LIBS=-lpthread -lSDL2 -lSDL2_image

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: QuienEsQuienApp

QuienEsQuienApp: $(OBJ) main.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o QuienEsQuienApp
