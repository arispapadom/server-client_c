all: server client

server: server.c MyHeader.h
	gcc server.c -o server

client: client.c MyHeader.h
	gcc client.c -o client
