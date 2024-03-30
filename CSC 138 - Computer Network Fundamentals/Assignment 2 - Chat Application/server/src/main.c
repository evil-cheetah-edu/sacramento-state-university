#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>

#define PORT "12345"
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 30

/**
 * Function: initialize_server
 * ----------------------------
 * Initializes the server to accept client connections.
 *
 * returns: The socket file descriptor of the listening server socket, or -1 if an error occurred.
 *
 * Task: Implement this function to create a server socket that listens on PORT. This involves
 * creating a socket, setting it to allow multiple connections, binding it to the correct port,
 * and starting to listen for client connections.
 */
int initialize_server();

/**
 * Function: accept_new_connection
 * -------------------------------
 * Accepts a new client connection.
 *
 * master_socket: The socket file descriptor of the server's listening socket.
 * client_socket: Array of client sockets.
 * max_clients: Maximum number of clients that can connect to the server.
 *
 * Task: Complete this function to accept a new client connection, add the new client socket
 * to the client_socket array, and print out the connection details. Make sure to handle the
 * case where the server is full and cannot accept more clients.
 */
void accept_new_connection(int master_socket, int client_socket[], int max_clients);

/**
 * Function: handle_client_activity
 * --------------------------------
 * Handles activity from an existing client connection.
 *
 * client_socket: Array of client sockets.
 * read_fds: The set of socket descriptors ready to be read.
 * max_clients: Maximum number of clients that can connect to the server.
 *
 * Task: Implement this function to read a message from a client socket indicated as ready in
 * read_fds, and broadcast that message to all other connected clients. If a client has
 * disconnected, close the socket and remove it from the client_socket array.
 */
void handle_client_activity(int client_socket[], fd_set read_fds, int max_clients);

/**
 * Function: send_to_all_other_clients
 * -----------------------------------
 * Sends a message to all clients except the sender.
 *
 * sender_sock: Socket descriptor of the client that sent the message.
 * client_sockets: Array of client sockets.
 * max_clients: Maximum number of clients.
 * msg: Message to be broadcast.
 *
 * Task: Write this function to loop through all connected clients and send the message
 * to each client except the sender. Ensure that the message is properly formatted and
 * sent correctly.
 */
void send_to_all_other_clients(int sender_sock, int client_sockets[], int max_clients, char *msg);

// Additional helper functions or definitions as necessary.


int main()
{
    int master_socket, client_socket[MAX_CLIENTS], max_sd;
    fd_set readfds;

    // Step 1: Initialize the server to start listening for connections.
    // TODO: Call initialize_server and check for errors.
    master_socket = initialize_server();

    if ( master_socket == -1 )
    {
        fprintf(stderr, "An error occurred while initializing socket! Terminating...\n");
        exit(1);
    }

    // Initialize client_socket array to 0.
    // TODO: Zero out the client_socket array to indicate no clients are connected initially.
    memset(client_socket, 0, sizeof(client_socket));

    // Main server loop.
    while ( 1 )
    {
        // Step 2: Clear and setup file descriptor sets.
        // TODO: Use FD_ZERO and FD_SET on readfds and add master_socket and all client sockets.
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (int i = 0; i < MAX_CLIENTS; ++i)
        {
            if ( client_socket[i] > 0 )
            {
                FD_SET(client_socket[i], &readfds);
            }

            if ( client_socket[i] > max_sd )
            {
                max_sd = client_socket[i];
            }
        }

        // Step 3: Wait for activity on any socket using select.
        // TODO: Use select to wait for activity and check for errors or interruptions.
        if ( select(max_sd + 1, &readfds, NULL, NULL, NULL) == -1 )
        {
            fprintf(stderr, "Error while selecting Socket Descriptor...\n");
            perror("select");
            exit(1);
        }

        // Step 4: Check for new connections on the master_socket.
        // TODO: If there's activity on master_socket, call accept_new_connection.
        if ( FD_ISSET(master_socket, &readfds) )
        {
            accept_new_connection(master_socket, client_socket, MAX_CLIENTS);
            fprintf(stdout, "Client Accepted...\n");
        }

        // Step 5: Handle IO activity from clients.
        // TODO: Loop through client sockets, checking each for activity. If any, call handle_client_activity.
        for (int i = 0; i <= MAX_CLIENTS; ++i)
        {
            if ( FD_ISSET(client_socket[i], &readfds) )
            {
                handle_client_activity(client_socket, readfds, MAX_CLIENTS);
            }

            if ( client_socket[i] > max_sd )
            {
                max_sd = client_socket[i];
            }
        }
        // Step 6: Clean up disconnected clients.
        // TODO: Inside handle_client_activity, ensure proper cleanup of disconnected client sockets.
    }

    // Clean up before shutting down the server.
    // TODO: Close all active client sockets and the master socket before exiting.
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if ( client_socket[i] != 0 )
            close(client_socket[i]);
    }
    
    close(master_socket);

    return 0;
}



int initialize_server()
{
    int rv;
    int socket_fd;
    int socket_option = 1; /// reuse port

    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags    = AI_PASSIVE;

    if ( (rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0 )
    {
        fprintf(
            stderr,
            "Failed getting Address Info...\n"
            "selectserver: %s\n", gai_strerror(rv)
        );
        perror("getaddrinfo");
        return -1;
    }

    for (p = ai; p != NULL; p = p->ai_next)
    {
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if ( socket_fd < 0 )
        {
            continue;
        }

        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &socket_option, sizeof(socket_option));

        if ( bind(socket_fd, p->ai_addr, p->ai_addrlen) < 0 )
        {
            close(socket_fd);
            continue;
        }

        break;
    }

    if ( p == NULL )
    {
        fprintf(stderr, "Failed to Bind...\n");
        return -1;
    }

    if ( listen(socket_fd, 10) == -1 )
    {
        fprintf(stderr, "Failed to start listening on a Socket...\n");
        perror("listen");
        return -1;
    }

    printf("Application is Listening on Port %s...\n", PORT);

    return socket_fd;
}


void accept_new_connection(int master_socket, int client_socket[], int max_clients)
{
    int client_sd;
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);

    if ( (client_sd = accept(master_socket, (struct sockaddr *)&client_address, &address_length)) == -1 )
    {
        fprintf(stderr, "Failed accepting new Client...\n");
        perror("accept");
        exit(1);
    }

    fprintf(
        stdout,
        "Client attempts to connect:\n"
        "   SD:   %d\n"
        "   IP:   %s\n"
        "   Port: %d\n",
        client_sd, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port)
    );

    for (int i = 0; i < max_clients; ++i)
    {
        if ( client_socket[i] == 0 )
        {
            client_socket[i] = client_sd;
            return;
        }
    }

    char *full_list_message = "Server is full... Please, try another time...\n";
    send(client_sd, full_list_message, strlen(full_list_message), 0);
    close(client_sd);
}


void handle_client_activity(int client_socket[], fd_set read_fds, int max_clients)
{
    int  input_size;
    char buffer[BUFFER_SIZE];

    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);

    for (int i = 0; i < max_clients; ++i)
    {
        int client_sd = client_socket[i];

        if ( !FD_ISSET(client_sd, &read_fds) )
        {
            continue;
        }

        if ( (input_size = read(client_sd, buffer, BUFFER_SIZE)) <= 0 )
        {
            getpeername(client_sd, (struct sockaddr*)&client_address, &address_length);
            
            fprintf(
                stdout,
                "Client disconnected:\n"
                "   SD:   %d\n"
                "   IP:   %s\n"
                "   Port: %d\n",
                client_sd, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port)
            );

            close(client_sd);
            client_socket[i] = 0;
        }
        else
        {
            buffer[input_size] = '\0';
            fprintf(
                stdout,
                "Payload Received:\n"
                "   Socket   %d\n"
                "   Payload: %s",
                client_sd, buffer 
            );
            send_to_all_other_clients(client_sd, client_socket, max_clients, buffer);
        }
    }
}


void send_to_all_other_clients(int sender_sock, int client_sockets[], int max_clients, char *msg)
{
    for (int i = 0; i < max_clients; ++i)
    {
        if (
            client_sockets[i] != sender_sock &&
            client_sockets[i] != 0
        )
        {
            send(client_sockets[i], msg, strlen(msg), 0);
        }
    }
}