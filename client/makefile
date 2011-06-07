network: client.o sockets.o
	gcc -g -lpthread client.o sockets.o -o im_cli
client.o: client.c client.h
	gcc -g -c client.c
sockets.o: sockets.c sockets.h
	gcc -g -c sockets.c
clean:
	rm *.o


