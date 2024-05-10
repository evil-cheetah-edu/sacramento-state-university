#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "httpserve.h"

#include <sys/stat.h>


#define HTTP_METHOD_LENGTH    (10)
#define URI_PATH_LENGTH     (2048)
#define HTTP_VERSION_LENGTH   (10)

#define WEB_ROOT_PATH       ("./www")


int main(int argc, char *argv[]) {
    // TODO: Parse command line arguments to override default port if necessary
    start_server(SERVER_PORT);
    return 0;
}


void start_server(int port)
{
    // TODO: Implement server initialization:
    // 1. Create a socket.
    int socket = create_socket(port);
    handle_connections(socket);
    // 2. Bind the socket to a port.
    // 3. Listen on the socket.
    // 4. Enter a loop to accept and handle connections.
}


int create_socket(int port)
{
    int socket_option = 1; // reuse port

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socket_fd < 0)
    {
        fprintf(stderr, "Failed creating a socket...\n");
        perror("socket");
        exit(1);
    }


    if (
        setsockopt(
            socket_fd,
            SOL_SOCKET,
            SO_REUSEADDR,
            &socket_option, sizeof(socket_option)
        ) < 0
    )
    {
        fprintf(stderr, "Failed setting socket options...\n");
        perror("setsockopt");
        exit(1);
    }

    struct sockaddr_in server_address;
    
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (
        bind(
            socket_fd,
            (struct sockaddr *)&server_address,
            sizeof(server_address)
        ) < 0
    )
    {
        fprintf(stderr, "Failed to Bind...\n");
        perror("bind");
        close(socket_fd);
        exit(1);
    }

    /// TODO: Pull from Environment Variable
    int backlog_connections = 10;

    if (
        listen(socket_fd, backlog_connections) < 0
    )
    {
        fprintf(stderr, "Failed to start listening on a Socket...\n");
        perror("listen");
        close(socket_fd);
        exit(1);
    }

    return socket_fd;
}


void handle_connections(int server_sock)
{
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);


    while (1)
    {
        int client_sd = accept(
            server_sock,
            (struct sockaddr *)&client_address,
            &address_length
        );

        if ( client_sd < 0 )
        {
            fprintf(stderr, "Failed to accept request from client...\n");
            perror("accept");
            close(client_sd);
            continue;
        }

        process_request(client_sd);

        close(client_sd);
    }
}


void process_request(int client_sock)
{
    char request[BUFFER_SIZE];

    memset(request, 0, sizeof(request));

    int read_size = read(client_sock, request, sizeof(request) - 1);

    if ( read_size < 0 )
    {
        fprintf(stderr, "Failed to read the client request...\n");
        perror("read");
        return;
    }

    request[BUFFER_SIZE] = '\0';

    char method[HTTP_METHOD_LENGTH],
         path[URI_PATH_LENGTH],
         version[HTTP_VERSION_LENGTH];

    if (
        sscanf(request, "%s %s %s", method, path, version) != 3
    ) 
    {
        fprintf(stderr, "Failed to parse request\n");
        return;
    }
    
    /// TODO: Remove Debug Info
    printf("Method:  `%s`\n", method);
    printf("URI:     `%s`\n", path);
    printf("VERSION: `%s`\n", version);

    if ( strcmp(method, "GET") == 0 )
    {
        handle_get_request(client_sock, path);
    }
    else if ( strcmp(method, "POST") == 0 )
    {
        handle_post_request(client_sock, path);
    }
    else
    {
        const char *response =
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Content-Length: 0\r\n\r\n";

        write(client_sock, response, strlen(response));
    }
}


void handle_get_request(int client_sock, const char* path)
{
    char file_path[256];
    char response_header[1024];

    struct stat file_stat;

    snprintf(file_path, sizeof(file_path), "%s%s", WEB_ROOT_PATH, path);

    /// TODO: Remove Debug Information
    printf("   Path: %s\n", file_path);

    FILE *file = fopen(file_path, "rb");

    if ( file == NULL )
    {
        printf("File does not exist");
    }
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
