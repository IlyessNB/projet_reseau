default:
	clear
	gcc -Wall client_memcpy.c -o client_memcpy -g -lpthread `pkg-config --cflags --libs glib-2.0`
