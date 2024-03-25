#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT 12345
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


int main() {
    int master_socket, client_socket[MAX_CLIENTS], max_sd;
    fd_set readfds;

    // Step 1: Initialize the server to start listening for connections.
    // TODO: Call initialize_server and check for errors.

    // Initialize client_socket array to 0.
    // TODO: Zero out the client_socket array to indicate no clients are connected initially.

    // Main server loop.
    while (1) {
        // Step 2: Clear and setup file descriptor sets.
        // TODO: Use FD_ZERO and FD_SET on readfds and add master_socket and all client sockets.

        // Step 3: Wait for activity on any socket using select.
        // TODO: Use select to wait for activity and check for errors or interruptions.

        // Step 4: Check for new connections on the master_socket.
        // TODO: If there's activity on master_socket, call accept_new_connection.

        // Step 5: Handle IO activity from clients.
        // TODO: Loop through client sockets, checking each for activity. If any, call handle_client_activity.

        // Step 6: Clean up disconnected clients.
        // TODO: Inside handle_client_activity, ensure proper cleanup of disconnected client sockets.
    }

    // Clean up before shutting down the server.
    // TODO: Close all active client sockets and the master socket before exiting.
    
    return 0;
}