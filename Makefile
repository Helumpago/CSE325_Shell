
CC=gcc
ARGS=-g -Wall
OUT=shell

main:
	$(CC) $(ARGS) shell.c -o $(OUT)

clean:
	rm -f $(OUT) *.o