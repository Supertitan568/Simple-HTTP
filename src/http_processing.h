#ifndef HTTP_PROCESSING_H
#define HTTP_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct http_request{
  char type;
  char* html_page_path;
}http_request;

http_request* parse_http_request(char* client_message);
int handle_GET_request(http_request* client_request, char** full_http_message);
#endif // !
