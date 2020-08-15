CC = gcc
CFLAGS = -Wall -pedantic

all: skip_list/skip_list.c test_skip_list.c benchmark.c   
	$(CC) $(CFLAGS) test_skip_list.c skip_list/skip_list.c -o test_skip_list 	
	$(CC) $(CFLAGS) benchmark.c skip_list/skip_list.c -o benchmark 

clean: 
	rm -f test_skip_list 
	rm -f benchmark	
