network: server.o sockets.o
	gcc -g -lpthread server.o sockets.o -o server
server.o: server.c server.h
	gcc -g -c server.c
sockets.o: sockets.c sockets.h
	gcc -g -c sockets.c
clean:
	rm *.o


