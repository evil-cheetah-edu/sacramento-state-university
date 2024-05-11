#ifndef HTTPSERVE_SSL_H
#define HTTPSERVE_SSL_H


#include <stdio.h>  // For size_t


// Server configuration constants
#define SERVER_PORT 8080
#define BUFFER_SIZE 16384


// Start the server on a specified port
void start_server(int port);

// Create a TCP socket and configure it
int create_socket(int port);

// Determine the MIME type based on the file extension
const char* get_mime_type(const char *filename);


#endif // HTTPSERVE_SSL_H
