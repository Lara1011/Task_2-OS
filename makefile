CC = gcc
AR = ar
FLAGS = -Wall -g -fPIC

default: stshell cmp copy encode decode codecA.so codecB.so

all: stshell cmp copy encode decode codecA.so codecB.so

stshell: stshell.c
	$(CC) $(FLAGS) -o stshell stshell.c

cmp: cmp.c
	$(CC) $(FLAGS) -o cmp cmp.c

copy: copy.c
	$(CC) $(FLAGS) -o copy copy.c

encode: encode.c
	$(CC) $(FLAGS) -o encode encode.c -ldl

decode: decode.c
	$(CC) $(FLAGS) -o decode decode.c -ldl

codecA.so: codecA.c
	$(CC) $(FLAGS) -shared codecA.c -o codecA.so

codecB.so: codecB.c
	$(CC) $(FLAGS) -shared codecB.c -o codecB.so

.PHONY: clean all

clean:
	rm -f *.o *.so stshell cmp copy encode decode