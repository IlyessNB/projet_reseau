default:
	clear
	gcc -Wall main.c -o main -g -lpthread `pkg-config --cflags --libs glib-2.0`
