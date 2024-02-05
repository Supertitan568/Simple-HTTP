#include "http_processing.h"
#include <stdio.h>
#include <string.h>
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

static int load_html_document(const char* html_page_path, char** file_contents){
  FILE* html_file = fopen(html_page_path, "r");
  size_t file_length = 0;

  fseek(html_file, 0 , SEEK_END);
  file_length = ftell(html_file);

  rewind(html_file);

  *file_contents = (char*) malloc(file_length);

  for(char* current_char = *file_contents; !feof(html_file); current_char++){
    *current_char = getc(html_file);
  }
  
  fclose(html_file);
  return file_length;
}

static const char* get_file_extension(const char* file_path){
  const char* dot_ptr = strrchr(file_path, '.');
  if(!dot_ptr || dot_ptr == file_path){
    return NULL;
  }
  return dot_ptr + 1;
}

static const char* get_mime_type(const char* file_path){
  const char* file_extension = get_file_extension(file_path);
  if(strcmp(file_extension, "html") == 0){
    return "text/html";
  }
  else if(strcmp(file_extension, "js") == 0){
    return "text/js";
  }
  else if(strcmp(file_extension, "txt") == 0){
    return "text/plain";
  }
  else if(strcmp(file_extension, "png") == 0){
    return "image/png";
  }
  else if(strcmp(file_extension, "jpeg") == 0 || strcmp(file_extension, "jpg") == 0){
    return "image/jpeg";
  }
  else{
    return "application/octet-stream";
  }
}

int handle_GET_request(http_request* client_request, char** full_http_message){
  int message_length = 0;
  if(strcmp(client_request->html_page_path, "/") == 0){
    char* default_page = "/index.html";
    free(client_request->html_page_path);
    client_request->html_page_path = (char*) malloc(strlen(default_page) + 1);
    strcpy(client_request->html_page_path, default_page);
  }
  if(access(client_request->html_page_path + 1, F_OK) == 0){
    char http_header[70];
    snprintf(http_header, 70, "HTTP/1.1 200 OK\r\nContent-Type:%s\r\n\r\n", get_mime_type(client_request->html_page_path));
    char* html_document = NULL;
    int file_length = load_html_document(client_request->html_page_path + 1, &html_document);
    message_length = strlen(http_header) + file_length + 1; 
    *full_http_message = (char*) malloc(message_length);
    strcpy(*full_http_message, http_header);
    memcpy(*full_http_message + strlen(http_header), html_document, file_length);
  
    free(html_document);
  }
  else{
    char* tmp_string = "HTTP/1.1 404 Not Found\r\n\r\n";
    *full_http_message = (char*) malloc(strlen(tmp_string) + 1);
    strcpy(*full_http_message, tmp_string);

  }
  return message_length;
} 
