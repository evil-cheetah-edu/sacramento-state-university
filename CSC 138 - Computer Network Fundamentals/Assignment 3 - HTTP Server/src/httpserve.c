#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "httpserve.h"

#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <sys/stat.h>


#define WITHOUT_SERVER_PORT   (1)
#define WITH_SERVER_PORT      (2)
#define SERVER_PORT_ARG_INDEX (1)

#define HTTP_METHOD_LENGTH  (10)
#define URI_PATH_LENGTH     (2048)
#define HTTP_VERSION_LENGTH (10)

#define WEB_ROOT_PATH       ("./www")

#define BACKLOG_CONNECTIONS (10)

#define LOGGER_STREAM       (stdout)
#define LOG_BUFFER_SIZE     (1024)

/// TODO: Rework into Enums
#define DEBUG               ("\033[95m DEBUG \033[0m")
#define INFO                ("\033[94m INFO \033[0m")
#define WARN                ("\033[33m WARN \033[0m")
#define ERROR               ("\033[91m ERROR \033[0m")
#define FATAL               ("\033[31m FATAL \033[0m")


void _BadRequestException(int client_sd);
void _ForbiddenException(int client_sd);
void _NotFoundException(int client_sd);
void _MethodNotAllowedException(int client_sd);
void _UnsupportedMediaTypeException(int client_sd);
void _InternalServerErrorException(int client_sd);


void logger(const char *log_level, const char *message);
void loggerf(const char *log_level, const char *format, ...);
int is_within_base_dir(const char *requested_path);
int is_port_number(char *input);


int main(int argc, char *argv[])
{
    if (
        argc != WITH_SERVER_PORT    &&
        argc != WITHOUT_SERVER_PORT
    )
    {
        logger(FATAL, "Usage: httpserve [optional SERVER_PORT]");
        exit(1);
    }
    
    int server_port;

    if ( argc == WITH_SERVER_PORT )
    {
        if ( !is_port_number(argv[SERVER_PORT_ARG_INDEX]) )
        {
            logger(FATAL, "Server Port is not an Integer...");
            logger(FATAL, "Usage: httpserve [optional SERVER_PORT]");
            exit(1);
        }

        server_port = atoi(argv[SERVER_PORT_ARG_INDEX]);
    }

    if ( argc == WITHOUT_SERVER_PORT )
    {
        server_port = SERVER_PORT;
    }

    // TODO: Parse command line arguments to override default port if necessary
    start_server(server_port);

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
        logger(FATAL, "Failed creating a socket...");
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
        logger(FATAL, "Failed setting socket options...");
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
        logger(FATAL, "Failed to Bind...");
        perror("bind");
        close(socket_fd);
        exit(1);
    }

    if (
        listen(socket_fd, BACKLOG_CONNECTIONS) < 0
    )
    {
        logger(FATAL, "Failed to start listening on a Socket...");
        perror("listen");
        close(socket_fd);
        exit(1);
    }

    loggerf(INFO, "Application is listening on port ':%d'...", port);

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
            logger(ERROR, "Failed to accept request from client...");
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

    request[read_size] = '\0';

    if ( read_size < 0 )
    {
        logger(ERROR, "Failed to read the client request...");
        perror("read");

        _BadRequestException(client_sock);

        return;
    }

    char method[HTTP_METHOD_LENGTH],
         path[URI_PATH_LENGTH],
         version[HTTP_VERSION_LENGTH];

    if (
        sscanf(request, "%s %s %s", method, path, version) != 3
    ) 
    {
        loggerf(WARN, "Failed to parse request | Payload: %s", request);
        perror("sscanf");

        _BadRequestException(client_sock);

        return;
    }

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

    _MethodNotAllowedException(client_sock);

    loggerf(INFO, "Method: %s | Payload: %s", method, request);
}


void handle_get_request(int client_sock, const char* path)
{
    char file_path[PATH_MAX];

    if ( strcmp(path, "/") == 0 )
    {
        logger(DEBUG, "Mapping '/' to '/index.html'");
        path = "/index.html";
    }

    char response_header[1024];

    // Resolve the absolute path and check for directory traversal
    if ( !is_within_base_dir(path) )
    {
        loggerf(ERROR, "Not Found or Directory Traversal | Attempt: %s", path);
        _ForbiddenException(client_sock);
        return;
    }

    snprintf(file_path, sizeof(file_path), "%s%s", WEB_ROOT_PATH, path);

    FILE *file = fopen(file_path, "rb");

    if ( !file )
    {
        loggerf(WARN, "File Not Found: %s", path);
        _NotFoundException(client_sock);
        return;
    }

    /// Get filesize
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *buffer = malloc(file_size);

    if ( !buffer )
    {
        logger(FATAL, "Memory allocation error...");
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

    loggerf(INFO, "Status: %d | Served File: %s", 200, file_path);
    
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
 * 
 * For more information, see:
 * @see {@link https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types}
 * 
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
 * @brief Returns a `400 Bad Request` HTTP error to the Client
 * 
 * For more information, see:
 * @see {@link https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/400}
 *
 * @param client_sd Client Socket Descriptor
**/
void _BadRequestException(int client_sd)
{
    send_response(
        client_sd,
        "HTTP/1.1 400 Not Found",
        get_mime_type(""),
        NULL,
        0
    );   
}


/**
 * @brief Returns a `404 Not Found` HTTP error to the Client
 * 
 * For more information, see:
 * @see {@link https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/404}
 * 
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
 * @brief Returns a `403 Forbidden` HTTP error to the Client
 * 
 * For more information, see:
 * @see {@link https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/403}
 * 
 * @param client_sd Client Socket Descriptor
**/
void _ForbiddenException(int client_sd)
{
    send_response(
        client_sd,
        "HTTP/1.1 403 Forbidden",
        get_mime_type(""),
        NULL,
        0
    );   
}


/**
 * @brief Returns a `405 Method Not Allowed` HTTP error to the Client
 * 
 * For more information, see:
 * @see {@link https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/405}
 * 
 * @param client_sd Client Socket Descriptor
**/
void _MethodNotAllowedException(int client_sd)
{
    send_response(
        client_sd,
        "HTTP/1.1 405 Method Not Allowed",
        get_mime_type(""),
        NULL,
        0
    );
}


/**
 * @brief Returns a `415 Unsupported Media Type` HTTP error to the Client.
 * 
 * For more information, see:
 * @see {@link https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/415}
 * 
 * @param client_sd Client Socket Descriptor
**/
void _UnsupportedMediaTypeException(int client_sd)
{
    send_response(
        client_sd,
        "HTTP/1.1 405 Method Not Allowed",
        get_mime_type(""),
        NULL,
        0
    );
}


/**
 * @brief Returns a `500 Internal Server Error` HTTP error to the Client
 * 
 * For more information, see:
 * @see {@link https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/500}
 *
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


/**
 * @brief Logs information out in the Standardized format
 * 
 * For more information, see:
 * @see {@link https://devhints.io/datetime}
 *
 * @param log_level Log Level, aka Severity 
 * @param message Logging message
**/
void logger(const char *log_level, const char *message)
{
    char stime[LOG_BUFFER_SIZE];

    time_t now           = time(NULL);
    struct tm *time_info = localtime(&now);

    strftime(
        stime, sizeof(stime),
        "%A, %b %d %Y @ %H:%M:%S%Z",
        time_info
    );

    fprintf(
        LOGGER_STREAM,
        "%s | [%s]: %s\n",
        stime, log_level, message
    );
}



/**
 * @brief Logs formatted message out in the Standardized format
 * 
 * For more information, see:
 * @see {@link https://devhints.io/datetime}
 *
 * @param log_level Log Level, aka Severity 
 * @param format F-String(Formatted String)
 * @param args Arguments that have to be placed in the F-Stringååå
**/
void loggerf(const char *log_level, const char *format, ...)
{
    char message[LOG_BUFFER_SIZE];

    va_list args;
    va_start(args, format);

    vsnprintf(message, sizeof(message), format, args);

    va_end(args);

    logger(log_level, message);
}

/**
 * @brief Directory Sanitization Check
 * 
 * Checks Requested Path for Traversal Attack Vulnerability
 * 
 * @param requested_path Path to be Validated
**/
int is_within_base_dir(const char *requested_path)
{
    char resolved_base[PATH_MAX];
    char resolved_path[PATH_MAX];

    /// Check If WebRoot Exists
    if ( realpath(WEB_ROOT_PATH, resolved_base) == NULL )
    {
        loggerf(ERROR, "Failed resolving WebRoot('%s')", WEB_ROOT_PATH);
        perror("realpath (base_dir)");
        return -1;
    }

    /// Combine 'WebRoot/RequestedPath`
    char full_path[PASS_MAX];
    snprintf(full_path, sizeof(full_path), "%s%s", WEB_ROOT_PATH, requested_path);

    // Resolve Path to Avoid Traversal Attack
    if ( realpath(full_path, resolved_path) == NULL )
    {
        loggerf(ERROR, "Failed resolving File Full('%s')", full_path);
        perror("realpath (full_path)");
        return -2;
    }

    /// Check if `ResolvedPath` Starts with `WebRoot`
    return strncmp(resolved_base, resolved_path, strlen(resolved_base)) == 0;
}


/**
 * @brief Checks if passed string is a Port Number or Not
 *
 * @param input string to be checked
**/
int is_port_number(char *input)
{
    for (unsigned int i = 0; input[i] != '\0'; ++i)
    {
        if ( !isdigit(input[i]) )
        {
            return 0;
        }
    }

    return 1;
}