#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "httpserve.h"


int main(int argc, char *argv[]) {
    // TODO: Parse command line arguments to override default port if necessary
    start_server(SERVER_PORT);
    return 0;
}


void start_server(int port) {
    // TODO: Implement server initialization:
    // 1. Create a socket.
    // 2. Bind the socket to a port.
    // 3. Listen on the socket.
    // 4. Enter a loop to accept and handle connections.
}


int create_socket(int port) {
    // TODO: Create and configure a TCP socket
    // Return the socket descriptor
}


void handle_connections(int server_sock) {
    // TODO: Implement connection handling:
    // 1. Accept a new connection.
    // 2. Handle the request in a separate function.
    // 3. Close the connection.
}


void process_request(int client_sock) {
    // TODO: Implement request processing:
    // 1. Read the request from the client.
    // 2. Parse the HTTP method, path, and protocol version.
    // 3. Depending on the method, call handle_get_request, handle_head_request, or handle_post_request.
}


void handle_get_request(int client_sock, const char* path) {
    // TODO: Handle GET request:
    // 1. Map the path to a file.
    // 2. Check if the file exists.
    // 3. Read the file and send it with an appropriate response header.
}


void handle_head_request(int client_sock, const char* path) {
    // TODO: Handle HEAD request similarly to GET but without sending the file content.
}


void handle_post_request(int client_sock, const char* path) {
    // TODO: Handle POST request:
    // 1. If the path is to a CGI script, execute the script and send the output as the response.
    // 2. If not, send a 404 Not Found response.
}


void send_response(int client_sock, const char *header, const char *content_type, const char *body, int body_length) {
    // TODO: Compile and send a full HTTP response.
    // Include the header, content type, body, and any other necessary HTTP headers.
}


const char* get_mime_type(const char *filename) {
    // TODO: Return the MIME type based on the file extension.
}
