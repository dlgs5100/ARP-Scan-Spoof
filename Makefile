all: arp

arp: arp.o main.o
	gcc -o arp arp.o main.o

arp.o: arp.c
	gcc -c arp.c

main.o: main.c
	gcc -c main.c

clean:
	rm -f arp *.[ois]