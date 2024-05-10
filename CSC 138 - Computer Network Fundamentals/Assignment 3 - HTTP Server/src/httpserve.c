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

#include <openssl/ssl.h>
#include <openssl/err.h>


#define WITHOUT_SERVER_PORT   (1)
#define WITH_SERVER_PORT      (2)
#define SERVER_PORT_ARG_INDEX (1)

#define HTTP_METHOD_LENGTH  (10)
#define URI_PATH_LENGTH     (2048)
#define HTTP_VERSION_LENGTH (10)

#define WEB_ROOT_PATH       ("./www")
#define CGI_ROOT_PATH       ("./cgi-bin")

#define BACKLOG_CONNECTIONS (10)
#define ENVIRONMENT_BUFFER  (64)

#define LOGGER_STREAM       (stdout)
#define LOG_BUFFER_SIZE     (1024)

#define HEADER_END          ("\r\n\r\n")
#define CONTENT_LENGTH      ("Content-Length:")

#define CHILD   (0)
#define PARENT  (1)

/// TODO: Rework into Enums
#define DEBUG               ("\033[95m DEBUG \033[0m")
#define INFO                ("\033[94m INFO \033[0m")
#define WARN                ("\033[33m WARN \033[0m")
#define ERROR               ("\033[91m ERROR \033[0m")
#define FATAL               ("\033[31m FATAL \033[0m")

void _RedirectToIndex(int client_sd);
void _BadRequestException(int client_sd);
void _ForbiddenException(int client_sd);
void _NotFoundException(int client_sd);
void _MethodNotAllowedException(int client_sd);
void _UnsupportedMediaTypeException(int client_sd);
void _InternalServerErrorException(int client_sd);


void logger(const char *log_level, const char *message);
void loggerf(const char *log_level, const char *format, ...);

void start_ssl_server(int port);
void handle_ssl_connections(int server_socket, SSL_CTX *context);
void handle_ssl_session(SSL *ssl);


int is_within_base_dir(const char *requested_path);
int is_port_number(char *input);

char *extract_headers(const char *request);
char *extract_body(const char *request, int read_size, const char *headers, int client_sd);


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

    /// HTTP: start_server(server_port);
    start_ssl_server(server_port);

    return 0;
}

/**
 * @brief Starts Regular HTTP Server
**/
void start_server(int port)
{
    int socket = create_socket(port);

    handle_connections(socket);
}


/**
 * @brief Starts HTTPS Server 
**/
void start_ssl_server(int port)
{
    SSL_CTX *context;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    context = SSL_CTX_new(TLS_server_method());

    if ( !context )
    {
        logger(FATAL, "Unable to create TLS(SSL) Context...");
        exit(1);
    }

    SSL_CTX_set_ecdh_auto(context, 1);

    if (
        SSL_CTX_use_certificate_file(context, "server.crt", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(context,  "server.key", SSL_FILETYPE_PEM) <= 0
    )
    {
        logger(FATAL, "Failed to load Certificates...");
        exit(1);
    }

    int server_socket = create_socket(port);
    
    handle_ssl_connections(server_socket, context);

    SSL_CTX_free(context);
    EVP_cleanup();
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


void handle_ssl_connections(int server_sock, SSL_CTX *context)
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
            logger(ERROR, "Failed to accept connection from client...");
            perror("accept");
            close(client_sd);
            continue;
        }

        SSL *ssl = SSL_new(context);
        SSL_set_fd(ssl, client_sd);

        if ( SSL_accept(ssl) <= 0 )
        {
            logger(ERROR, "An error occurred during handshake...");
            SSL_free(ssl);
            close(client_sd);
            continue;
        }

        handle_ssl_session(ssl);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_sd);
    }
}


void process_request(int client_sock)
{
    char request[BUFFER_SIZE];

    memset(request, 0, sizeof(request));

    int read_size = recv(client_sock, request, sizeof(request) - 1, 0);

    if ( read_size <= 0 )
    {
        logger(ERROR, "Failed to read the client request...");
        perror("read");

        _BadRequestException(client_sock);

        return;
    }

    request[read_size] = '\0';

    char *headers = extract_headers(request);

    char method[HTTP_METHOD_LENGTH]   = { 0 },
         path[URI_PATH_LENGTH]        = { 0 },
         version[HTTP_VERSION_LENGTH] = { 0 };

    if (
        sscanf(headers, "%s %s %s", method, path, version) != 3
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

        free(headers);

        return;
    }

    if ( strcmp(method, "HEAD") == 0 )
    {
        handle_head_request(client_sock, path);

        free(headers);

        return;
    }
    
    if ( strcmp(method, "POST") == 0 )
    {
        char *body = extract_body(request, read_size, headers, client_sock);

        loggerf(DEBUG, "Processing: %s Method", method);

        handle_post_request(client_sock, path, headers, body);

        free(headers);
        free(body);

        return;
    }

    _MethodNotAllowedException(client_sock);

    loggerf(INFO, "Method: %s | Payload: %s", method, request);
}


void handle_ssl_session(SSL *ssl)
{
    char *request[BUFFER_SIZE];

    int byte_size = SSL_read(ssl, request, sizeof(request));

    if ( byte_size > 0 )
    {
        request[byte_size] = '\0';
        loggerf(INFO, "Received: %s", request);
        SSL_write(ssl, request, byte_size);
        return;
    }

    logger(FATAL, "Received nothing...");
}


void handle_get_request(int client_sock, const char* path)
{
    char file_path[PATH_MAX];

    if ( strcmp(path, "/") == 0 )
    {
        /// TODO: Rework Logger to Log Method Name
        logger(DEBUG, "Method: GET | Mapping '/' to '/index.html'");
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
        logger(FATAL, "Method: GET | Memory allocation error...");
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


/// TODO: Refactor HEAD and GET
void handle_head_request(int client_sock, const char* path)
{
    char file_path[PATH_MAX];

    if ( strcmp(path, "/") == 0 )
    {
        logger(DEBUG, "Method: HEAD | Mapping '/' to '/index.html'");
        path = "/index.html";
    }

    char response_header[1024];

    // Resolve the absolute path and check for directory traversal
    if ( !is_within_base_dir(path) )
    {
        loggerf(
            ERROR,
            "Method: HEAD | Not Found or Directory Traversal | "
            "Attempt: %s",
            path
        );

        _ForbiddenException(client_sock);
        return;
    }

    snprintf(file_path, sizeof(file_path), "%s%s", WEB_ROOT_PATH, path);

    FILE *file = fopen(file_path, "rb");

    if ( !file )
    {
        loggerf(WARN, "Method: HEAD | File Not Found: %s", path);
        _NotFoundException(client_sock);
        return;
    }
    
    send_response(
        client_sock,
        "HTTP/1.1 200 OK",
        get_mime_type(file_path),
        NULL,
        0
    );

    loggerf(INFO, "Status: %d | HEAD Request for File: %s", 200, file_path);
    
    fclose(file);
}


void handle_post_request(int client_sock, const char *path, char* headers, char* body)
{
    path = "./cgi-bin/testcgi";

    char content_length[ENVIRONMENT_BUFFER];
    sprintf(content_length, "CONTENT_LENGTH=%ld", strlen(body));

    putenv("REQUEST_METHOD=POST");
    putenv(content_length);

    loggerf(DEBUG, "Method: POST | Path: `%s`", path);

    /// TODO: Validate and Sanitize the Path
    if ( access(path, X_OK) == -1 )
    {
        loggerf(WARN, "Method: POST | File Not Found: %s", path);
        _NotFoundException(client_sock);
    }

    int pipe_stdin[2],
        pipe_stdout[2];

    pipe(pipe_stdin);
    pipe(pipe_stdout);

    pid_t pid = fork();

    if ( pid == CHILD )
    {
        dup2(pipe_stdin[CHILD], STDIN_FILENO);
        
        close(pipe_stdin[PARENT]);
        close(pipe_stdin[CHILD]); // Not used anymore

        dup2(pipe_stdout[PARENT], STDOUT_FILENO);

        close(pipe_stdout[CHILD]);
        close(pipe_stdout[PARENT]); // Not used anymore

        char script_path[PATH_MAX];
        sprintf(script_path, "%s", path);

        execl(script_path, script_path, (char *)NULL);

        _InternalServerErrorException(client_sock);
        loggerf(FATAL, "Error while running CGI Script: %s", path);
        exit(1);
    }

    else if ( pid > CHILD )
    {
        close(pipe_stdin[CHILD]);
        close(pipe_stdout[PARENT]);

        write(pipe_stdin[PARENT], body, strlen(body));
        close(pipe_stdout[PARENT]);

        char *response[BUFFER_SIZE];
        int byte_size;

        while ( (byte_size = read(pipe_stdout[CHILD], response, sizeof(response) - 1)) > 0 )
        {
            send(client_sock, response, byte_size, 0);
        }

        close(pipe_stdout[CHILD]);

        wait(NULL);

        _RedirectToIndex(client_sock);
    }

    else
    {
        _InternalServerErrorException(client_sock);
        logger(FATAL, "Failed creating a fork...");
        exit(1);
    }
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
 * @brief Redirects User to `/index.html`
 * 
 * For more information, see:
 * @see {@link https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/302}
 *
 * @param client_sd Client Socket Descriptor
**/
void _RedirectToIndex(int client_sd)
{
    send_response(
        client_sd,
        "HTTP/1.1 302 Found\r\n"
        "Location: /index.html\r\n\r\n",
        get_mime_type(""),
        NULL,
        0
    );   
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
 * @param args Arguments that have to be placed in the F-String
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
    char full_path[PATH_MAX];
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


char *extract_headers(const char *request)
{
    char* header_end = strstr(request, HEADER_END);

    /// Invalid or Incomplete Headers
    if (header_end == NULL)
    {
        return NULL;
    }

    int header_length = header_end - request + strlen(HEADER_END);

    char* headers = malloc(header_length + 1);

    /// Memory Allocation Failure
    if (headers == NULL)
    {
        return NULL;
    }

    strncpy(headers, request, header_length);

    headers[header_length] = '\0';

    return headers;
}


char *extract_body(const char *request, int read_size, const char *headers, int client_sd)
{
    const char *payload_size = strstr(headers, CONTENT_LENGTH);

    /// No Content Length in the Header
    if ( payload_size == NULL )
    {
        return NULL;
    }

    int content_length = atoi(payload_size + strlen(CONTENT_LENGTH));

    char *header_end = strstr(request, HEADER_END);

    /// Invalid Request or Header not Complete
    if ( header_end == NULL )
    {
        return NULL;
    }

    char *body_start = header_end + strlen(HEADER_END);
    int body_received = read_size - (body_start - request);

    if ( body_received >= content_length )
    {
        char* body = malloc(content_length + 1);

        if ( body == NULL )
        {
            return NULL;
        }

        memcpy(body, body_start, content_length);

        body[content_length] = '\0';

        return body;
    }
    
    char *body = malloc(content_length + 1);

    if ( body == NULL )
    {
        return NULL;
    }

    memcpy(body, body_start, body_received);

    int total_read      = body_received;
    int remaining_bytes = content_length - body_received;

    while (total_read < content_length)
    {
        int bytes_read = recv(client_sd, body + total_read, remaining_bytes, 0);

        if ( bytes_read <= 0 )
        {
            free(body);

            return NULL;
        }

        total_read      += bytes_read;
        remaining_bytes -= bytes_read;
    }

    body[content_length] = '\0';

    return body;
}
