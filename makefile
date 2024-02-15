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

install:
	cp bin/simple_http_server /usr/local/bin/
	cp services/simple_http_server.service /etc/systemd/system/
	useradd http-server
	systemctl daemon-reload
	
	mkdir /var/website
	cp bin/index.html /var/website/

uninstall:
	rm /usr/local/bin/simple_http_server
	rm /etc/systemd/system/simple_http_server.service
	userdel http-server

clean:
	rm $(OBJECTS) ./bin/simple_http_server
