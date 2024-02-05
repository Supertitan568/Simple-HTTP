CC := gcc
CFLAGS := -g
OBJECTS := http_processing.o main.o socket_handling.o
all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o ./bin/simple_http_server

main.o: ./src/main.c
	$(CC) $(CFLAGS) -c ./src/main.c -o ./main.o

http_processing.o: ./src/http_processing.c
	$(CC) $(CFLAGS) -c ./src/http_processing.c -o ./http_processing.o
socket_handling.o: ./src/socket_handling.c
	$(CC) $(CFLAGS) -c ./src/socket_handling.c -o ./socket_handling.o
clean:
	rm $(OBJECTS) ./bin/simple_http_server
