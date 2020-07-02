CC = gcc -o
CFLAGS = -Wall -lpthread `pkg-config --cflags --libs glib-2.0`
ALL = dazibao

dazibao : main.o
main.o : main.c main.h

.PHONY : clean
clean:
	rm -f dazibao *.o
