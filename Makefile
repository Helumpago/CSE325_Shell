
CC=clang
ARGS=-g -Wall
OUT=o.exe

main:
	$(CC) $(ARGS) shell.c -o shell

clean:
	rm -f *.exe *.o