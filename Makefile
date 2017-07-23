CC=gcc -std=gnu99

default: main
	
main:
	$(CC) rotating_cube.c -o rc
	
clean:
	rm rc

