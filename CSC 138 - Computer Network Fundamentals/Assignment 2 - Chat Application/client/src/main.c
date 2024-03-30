#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define PORT "12345"
#define BUFFER_SIZE 1024

#define PROGRAM_NAME (argv[0])
#define HOST         (argv[1])
#define USERNAME     (argv[2])


/**
 * Function: initialize_connection
 * -------------------------------
 * Establishes a connection to the chat server.
 *
 * server_hostname: a string representing the hostname or IP address of the server.
 * port: a string representing the port number on which the server is listening.
 *
 * returns: a socket file descriptor for the established connection, or -1 if an error occurred.
 *
 * Task: Complete this function to resolve the given hostname and port to an address
 * and establish a TCP connection. Use getaddrinfo to obtain the server's address and
 * connect to the first address you can.
 */
int initialize_connection(const char *server_hostname, const char *port);

/**
 * Function: handle_server_message
 * -------------------------------
 * Reads and processes messages received from the server.
 *
 * sock: the socket file descriptor connected to the server.
 *
 * Task: Implement this function to continuously listen for messages from the server
 * and print them to stdout. This function will be called whenever the select mechanism
 * indicates that the server socket has data to be read. Ensure the message is null-terminated
 * before printing.
 */
void handle_server_message(int sock);

/**
 * Function: handle_user_input
 * ---------------------------
 * Sends a user's message to the server.
 *
 * sock: the socket file descriptor connected to the server.
 * username: the user's username, which should be prepended to the message.
 *
 * Task: Implement this function to read a line of input from stdin, prepend the username
 * to it, and send it to the server. This function will be called whenever the select mechanism
 * indicates that there is user input to be read from stdin. Ensure proper formatting of the message
 * for readability by other clients.
 */
void handle_user_input(int sock, const char *username);


void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// The main function and any additional helper functions or definitions as necessary.


int main(int argc, char *argv[])
{
    // Step 1: Validate command-line arguments.
    // TODO: Ensure there are exactly three arguments: the executable name, the server's hostname, and the username.
    if ( argc != 3 )
    {
        fprintf(stderr, "Usage: %s <hostname> <username>\n", PROGRAM_NAME);
        exit(1);
    }

    // Step 2: Initialize connection to the server.
    // TODO: Call initialize_connection with the server's hostname and port number to establish a connection.
    int socket_fd = initialize_connection(HOST, PORT);

    if ( socket_fd == -1 )
    {
        exit(1);
    }

    // Step 3: Main loop - Use select() to multiplex between stdin (user input) and the socket (incoming messages).
    // TODO: Inside the loop, do the following:
    // a. Clear and set up the readfds set for select() with both stdin and the server socket.
    // b. Call select() to wait for activity on either stdin or the socket.
    // c. If select indicates activity on the server socket, call handle_server_message to process it.
    // d. If select indicates activity on stdin, call handle_user_input to read the user's message and send it to the server.
    fd_set master;
    fd_set read_fds;
    int fdmax;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(STDIN_FILENO, &master);
    FD_SET(socket_fd, &master);
    fdmax = socket_fd;

    while (1)
    {
        read_fds = master;

        if ( select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1 )
        {
            fprintf(stderr, "Error while selecting Socket Descriptor...\n");
            perror("select");
            exit(1);
        }

        if ( FD_ISSET(STDIN_FILENO, &read_fds) )
        {
            handle_user_input(socket_fd, USERNAME);
        }

        if ( FD_ISSET(socket_fd, &read_fds) )
        {
            handle_server_message(socket_fd);
        }
    }

    // Step 4: Clean up and close the socket before exiting.
    // TODO: Properly close the socket descriptor when the program is terminating.
    close(socket_fd);

    return 0; // Exit the program.
}


int initialize_connection(const char *server_hostname, const char *port)
{
    int status;
    int socket_fd = 0;
    struct addrinfo hints, *server_info, *p;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));

    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    // hints.ai_flags    = AI_PASSIVE;

    if ( (status = getaddrinfo(server_hostname, port, &hints, &server_info)) != 0 )
    {
        fprintf(
            stderr,
            "Failed getting the Host Information...\n"
            "getaddrinfo: %s\n",
            gai_strerror(status)
        );
        perror("getaddrinfo");
        return -1;
    }

    for (p = server_info; p != NULL; p = p->ai_next)
    {
        if ( (socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1 )
        {
            fprintf(stderr, "Failed creating a Socket...\n");
            perror("client: socket");
            continue;
        }

        if ( connect(socket_fd, p->ai_addr, p->ai_addrlen) == -1 )
        {
            fprintf(stderr, "Failed connecting to the Host...\n");
            perror("client: connect");
            close(socket_fd);
            continue;
        }

        break;
    }

    if ( p == NULL )
    {
        fprintf(stderr, "Failed to establish a connection...\n");
        return -1;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s));
    fprintf(stdout, "client: connecting to %s\n", s);
    
    freeaddrinfo(server_info);

    return socket_fd;
}


void handle_server_message(int sock)
{
    char buffer[BUFFER_SIZE];
    int  message_length;

    if ( (message_length = recv(sock, buffer, BUFFER_SIZE - 1, 0)) == -1 )
    {
        fprintf(stderr, "Failed while receiving the data from Server...\n");
        perror("recv");
        exit(1);
    }

    buffer[message_length] = '\0';
    
    fprintf(stdout, "%s", buffer);
}


void handle_user_input(int sock, const char *username)
{
    char message[BUFFER_SIZE];
    char payload[BUFFER_SIZE + 50];

    fgets(message, BUFFER_SIZE, stdin);
    sprintf(payload, "%s: %s", username, message);

    if ( send(sock, payload, strlen(payload), 0) == -1 )
    {
        fprintf(stderr, "Failed sending the data to the Server...\n");
        perror("send");
    }
}