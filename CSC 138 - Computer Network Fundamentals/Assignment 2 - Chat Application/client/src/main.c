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

// The main function and any additional helper functions or definitions as necessary.


int main(int argc, char *argv[]) {
    // Step 1: Validate command-line arguments.
    // TODO: Ensure there are exactly three arguments: the executable name, the server's hostname, and the username.

    // Step 2: Initialize connection to the server.
    // TODO: Call initialize_connection with the server's hostname and port number to establish a connection.

    // Step 3: Main loop - Use select() to multiplex between stdin (user input) and the socket (incoming messages).
    // TODO: Inside the loop, do the following:
    // a. Clear and set up the readfds set for select() with both stdin and the server socket.
    // b. Call select() to wait for activity on either stdin or the socket.
    // c. If select indicates activity on the server socket, call handle_server_message to process it.
    // d. If select indicates activity on stdin, call handle_user_input to read the user's message and send it to the server.

    // Step 4: Clean up and close the socket before exiting.
    // TODO: Properly close the socket descriptor when the program is terminating.

    return 0; // Exit the program.
}