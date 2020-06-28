default:
	clear
	gcc -Wall client_memcpy.c -o client_memcpy -g `pkg-config --cflags --libs glib-2.0`
