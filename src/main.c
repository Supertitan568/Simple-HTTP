#include <netinet/in.h>
#include "http_processing.h" 
#include <stdio.h>
#include <sys/socket.h>
#define PORT 80

void set_up_socket(int* server_fd, int* new_socket, struct sockaddr_in* address){
   int opt = 1;
   socklen_t addrlen = sizeof(*address);

   // Creating socket file descriptor
   if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
     perror("socket failed");
     exit(EXIT_FAILURE);
   }
 
   // Forcefully attaching socket to port 80
   if (setsockopt(*server_fd, SOL_SOCKET,
     SO_REUSEADDR | SO_REUSEPORT, &opt,
     sizeof(opt))) {
     perror("setsockopt");
     exit(EXIT_FAILURE);
    }
    (*address).sin_family = AF_INET;
    (*address).sin_addr.s_addr = INADDR_ANY;
    (*address).sin_port = htons(PORT);
 
    // Forcefully attaching socket to the port 80
  if (bind(*server_fd, (struct sockaddr*)address,sizeof(*address))< 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(*server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  if ((*new_socket = accept(*server_fd, (struct sockaddr*)address, &addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
}

void handle_new_client(){
  int server_fd, new_socket;
  ssize_t request_size;
  struct sockaddr_in address;
  char buffer[2048] = { 0 };

  set_up_socket(&server_fd, &new_socket, &address);
  request_size = read(new_socket, buffer, 2048 - 1);
  printf("http request size:%li\n", request_size);
  printf("%s", buffer);

  http_request* client_request = parse_http_request(buffer);

  char* full_http_request = NULL;
  switch(client_request->type){
    case 'G':
      full_http_request = handle_GET_request(client_request);
      break;

    default:
      full_http_request = "HTTP/1.1 501 Not Implemented\r\n";   
  } 
  
  send(new_socket, full_http_request, strlen(full_http_request), 0);
  printf("html file sent\n");
 
  close(new_socket);
  close(server_fd);
  free(client_request->html_page_path);
  free(client_request);
  free(full_http_request);
}

int main(int argc, char const* argv[]){
  handle_new_client();
  return 0;
}
