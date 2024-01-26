#include "http_processing.h"
#include <stdio.h>
#include <unistd.h>

http_request* parse_http_request(char* client_message){
  char* request_line = strtok(client_message, "\n");
  http_request* request = (http_request*) malloc(sizeof(http_request));
  char* request_type = strtok(request_line, " ");

  if(strcmp(request_type, "GET") == 0){
    request->type = 'G';
  }
  else if(strcmp(request_type, "HEAD")){
    request->type = 'H';
  }
  else if(strcmp(request_type, "POST")){
    request->type = 'P';
  }
  else if(strcmp(request_type, "PUT")){
    request->type = 'U';
  }
  else if(strcmp(request_type, "DELETE")){
    request->type = 'D';
  }
  else if(strcmp(request_type, "CONNECT")){
    request->type = 'C';
  }
  else if(strcmp(request_type, "OPTIONS")){
    request->type = 'O';
  }
  else if(strcmp(request_type, "TRACE")){
    request->type = 'T';
  }
  else{
    request->type = 0;
    request->html_page_path = NULL;
    return request;
  }
  //Error checking for the html path will be in the html loading function
  char* html_page_path = strtok(0, " ");
  
  request->html_page_path = (char*) malloc(strlen(html_page_path));
  strcpy(request->html_page_path, html_page_path);

  return request;
}

static char* load_html_document(const char* html_page_path){
  FILE* html_file = fopen(html_page_path, "r");
  size_t file_length = 0;

  fseek(html_file, 0 , SEEK_END);
  file_length = ftell(html_file);

  rewind(html_file);

  char* file_contents = (char*) malloc(file_length);

  for(char* current_char = file_contents; !feof(html_file); current_char++){
    *current_char = getc(html_file);
  }
  
  fclose(html_file);
  return file_contents;
}

char* handle_GET_request(http_request* client_request){
  char* full_http_messsage;
  if(access(client_request->html_page_path + 1, F_OK) == 0 && strcmp(client_request->html_page_path, "/")!= 0){
    char* http_header = "HTTP/1.1 200 OK\r\n";
    char* html_document = load_html_document(client_request->html_page_path + 1);
    full_http_messsage = (char*) malloc(strlen(http_header) + strlen(html_document) + 1);
    strcpy(full_http_messsage, http_header);
    strcat(full_http_messsage, html_document);
  
    free(html_document);
  }
  else{
    char* tmp_string = "HTTP/1.1 404 Not Found\r\n";
    full_http_messsage = (char*) malloc(strlen(tmp_string) + 1);
    strcpy(full_http_messsage, tmp_string);
  }
  return full_http_messsage;
} 
