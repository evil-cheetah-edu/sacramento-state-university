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

#define BACKLOG_CONNECTIONS   (10)


void _InternalServerErrorException(int client_sd);
void _NotFoundException(int client_sd);


int main(int argc, char *argv[])
{
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

    if (
        listen(socket_fd, BACKLOG_CONNECTIONS) < 0
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

        _InternalServerErrorException(client_sock);

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
        perror("sscanf");

        _InternalServerErrorException(client_sock);

        return;
    }
    
    /// TODO: Remove Debug Info
    printf("Method:  `%s`\n", method);
    printf("URI:     `%s`\n", path);
    printf("VERSION: `%s`\n", version);

    if ( strcmp(method, "GET") == 0 )
    {
        handle_get_request(client_sock, path);
        return;
    }
    
    if ( strcmp(method, "POST") == 0 )
    {
        handle_post_request(client_sock, path);
        return;
    }
    
    send_response(
        client_sock,
        "HTTP/1.1 405 Method Not Allowed",
        get_mime_type(""),
        NULL,
        0
    );
}


void handle_get_request(int client_sock, const char* path)
{
    char file_path[256];
    char response_header[1024];

    snprintf(file_path, sizeof(file_path), "%s%s", WEB_ROOT_PATH, path);

    /// TODO: Remove Debug Information
    /// TODO: Sanitize Path
    printf("   Path: %s\n", file_path);

    FILE *file = fopen(file_path, "rb");

    if ( !file )
    {
        _NotFoundException(client_sock);
        return;
    }

    /// Get filesize
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    const char *content_type = get_mime_type(file_path);
    
    char *buffer = malloc(file_size);

    if ( !buffer )
    {
        fprintf(stderr, "Memory allocation error...\n");
        perror("malloc");
        fclose(file);

        _InternalServerErrorException(client_sock);
        
        return;
    }

    /// TODO: Refactor by sending via Buffer
    /// Potential Issue: memory allocation error, when trying to
    /// serve large files or many users
    /// Current Limitations: function signatures that lie
    /// in specifications in `httpserve.h`
    fread(buffer, 1, file_size, file);
    
    send_response(
        client_sock,
        "HTTP/1.1 200 OK",
        get_mime_type(file_path),
        buffer,
        file_size
    );

    write(client_sock, response_header, strlen(response_header));
    
    free(buffer);
    fclose(file);
}


void handle_head_request(int client_sock, const char* path) {
    // TODO: Handle HEAD request similarly to GET but without sending the file content.
}


void handle_post_request(int client_sock, const char* path) {
    // TODO: Handle POST request:
    // 1. If the path is to a CGI script, execute the script and send the output as the response.
    // 2. If not, send a 404 Not Found response.
}


void send_response(int client_sock, const char *header, const char *content_type, const char *body, int body_length)
{
    char *response[BUFFER_SIZE];

    snprintf(
        response, sizeof(response),
        "%s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n",
        header, content_type, body_length
    );

    write(client_sock, response, strlen(response));

    write(client_sock, body, body_length);
}


/**
 * @brief Returns MIME Type for based on File Extension
 * @param filename Filename or Path to a file
**/
const char* get_mime_type(const char *filename)
{
    if ( strstr(filename, ".html") )
        return "text/html";

    if ( strstr(filename, ".css") )
        return "text/css";

    if ( strstr(filename, ".js") )
        return "application/javascript";

    if ( strstr(filename, ".png") )
        return "image/png";

    if ( strstr(filename, ".jpg") )
        return "image/jpg";

    if ( strstr(filename, ".jpeg") )
        return "image/jpeg";

    if ( strstr(filename, ".gif") )
        return "image/gif";
    
    return "text/plain";
}


/**
 * @brief Returns a `404 Not Found` HTTP error to the Client
 * @param client_sd Client Socket Descriptor
**/
void _NotFoundException(int client_sd)
{
    send_response(
        client_sd,
        "HTTP/1.1 404 Not Found",
        get_mime_type(""),
        NULL,
        0
    );   
}


/**
 * @brief Returns a `500 Internal Server Error` HTTP error to the Client
 * @param client_sd Client Socket Descriptor
**/
void _InternalServerErrorException(int client_sd)
{
    send_response(
        client_sd,
        "HTTP/1.1 500 Internal Server Error",
        get_mime_type(""),
        NULL,
        0
    );
}