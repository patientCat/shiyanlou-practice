CC = g++
CFLAGS = --std=c++11
all:clientmain servermain
servermain: server.o
	$(CC) $(CFLAGS) -o $@ server.o servermain.cpp
clientmain: client.o 
	$(CC) $(CFLAGS) -o $@ client.o clientmain.cpp
server.o:server.cpp server.hpp common.hpp
	$(CC) -o $@ -c $<
client.o:client.cpp client.hpp common.hpp
	$(CC) -o $@ -c $<
clean: 
	rm -rf servermain clientmain *.o
