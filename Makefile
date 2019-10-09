CC = gcc
TARGETS = hw

$(TARGETS) : 
	$(CC) hw2.c -o hw

clean : 
	rm hw
