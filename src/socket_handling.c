#include "sock_handling.h"

#define _OPEN_SYS
#define _OPEN_THREADS
#define PORT 80

static void set_up_socket(int* server_fd, struct sockaddr_in* address){
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
}

void* handle_new_client(void* arg){
  int request_size, new_socket = *((int*) arg); 
  char buffer[2048] = { 0 };

  request_size = read(new_socket, buffer, 2048 - 1);
  printf("%s", buffer);

  http_request* client_request = parse_http_request(buffer);
  char* full_http_request = NULL;
  int message_size;
  switch(client_request->type){
    case 'G':
      message_size = handle_GET_request(client_request, &full_http_request);
      break;

    default:
      full_http_request = "HTTP/1.1 501 Not Implemented\r\n\r\n";   
  } 
  
  send(new_socket, full_http_request, message_size, 0);
 
  close(new_socket);
  free((int*) arg);
  free(client_request->html_page_path);
  free(client_request);
  free(full_http_request);

  return NULL;
}

void run_server(){
  int server_fd;
  struct sockaddr_in server_address;
  socklen_t addrlen = sizeof(struct sockaddr_in);

  set_up_socket(&server_fd, &server_address);

  while(1){
    pthread_t thread_id;
    int* new_socket = (int*) malloc(sizeof(int));
    if ((*new_socket = accept(server_fd, (struct sockaddr*) &server_address, &addrlen)) < 0) {
      perror("accept");
      exit(1);
    }
    
    pthread_create(&thread_id, NULL, handle_new_client, (void *) new_socket);
    pthread_detach(thread_id);
  }
  close(server_fd);
}
