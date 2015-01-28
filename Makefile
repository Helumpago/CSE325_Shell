
CC=clang
ARGS=-g -Wall
OUT=o.exe

main:
	$(CC) $(ARGS) main.c -o o.exe

clean:
	rm -f *.exe *.o