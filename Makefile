all: 
	gcc -o client client.c
	gcc -o server server.c
client: 
	gcc -o client client.c
server: 
	gcc -o server server.c
clean:
	rm ./client
	rm ./server
