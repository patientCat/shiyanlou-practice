SOURCE = $(wildcard *.c)
EXCE = $(SOURCE:.c=)
all:server client
server:server.c
	gcc -o $@ $^
client:client.c
	gcc -o $@ $^
clean:
	rm -rf $(EXCE)
