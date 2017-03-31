CC= gcc
flags= -w -lrt -pthread

sort:
	$(CC) $(flags) -o sort threadedsort.c

clean:
	rm -f sort
