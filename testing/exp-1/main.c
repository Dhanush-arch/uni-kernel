/**
 * This code creates a simple webserver that pretends to write a input
 * if the size of the input is large, the default password is overwritten
 * This is known as buffer overflow 
**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "picohttpparser.h"

#define MAX_REQUEST_SIZE 4096
#define PORT 80

void handle_get_request(int client_sock, const char* path) {
    // Handle GET request logic here
    printf("GET\n");
    char response[512];
    snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    write(client_sock, response, strlen(response));
}

void handle_post_request(int client_sock, const char* path, const char* body, size_t body_size) {
    // Handle POST request logic here
    char response[512];
//    printf("POST\n");
//    printf("body: \n");
    int i = 0;
    while(body_size--){
        printf("%c", body[i]);
        i++;
    }
    snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    write(client_sock, response, strlen(response));
}

void handle_request(int client_sock, char* request_buffer, size_t request_size, char *input) {
    const char* method;
    const char* path;
    size_t method_len;
    size_t path_len;
    int minor_version;
    struct phr_header headers[100];
    size_t num_headers = sizeof(headers) / sizeof(headers[0]);
    char* body;
    size_t body_len;

    strcpy(input, "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");

    int ret = phr_parse_request(request_buffer, request_size, &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
    if (ret <= 0) {
        // Error parsing the request
        char response[512];
        snprintf(response, sizeof(response), "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
        write(client_sock, response, strlen(response));
        return;
    }

    // Get the request body
    body = request_buffer + ret;
    body_len = request_size - ret;

    if (strncmp(method, "GET", method_len) == 0) {
        handle_get_request(client_sock, path);
    } else if (strncmp(method, "POST", method_len) == 0) {
        handle_post_request(client_sock, path, body, body_len);
    } else {
        // Unsupported HTTP method
        char response[512];
        snprintf(response, sizeof(response), "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n");
        write(client_sock, response, strlen(response));
    }
}

int main(int argc, char* argv[]) {
    int server_sock, client_sock, optval;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char request_buffer[MAX_REQUEST_SIZE];
    int ret, i;
    char inputpassword[50] = "";
    char password[50] = "password";
    printf("\nExp 1 where buffer overflow occurs\n\n");
    // Create the server socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(1);
    }

    // Set the SO_REUSEADDR option to avoid "Address already in use" errors
    optval = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    // Bind the server socket to the specified port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
   
    if (bind(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) < 0) {
        perror("listen");
        exit(1);
    }

    // Accept incoming connections and handle requests
    while (1) {
        client_addr_len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*) &client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }

        // Receive the request from the client
        ret = read(client_sock, request_buffer, sizeof(request_buffer));
        if (ret < 0) {
            perror("read");
            close(client_sock);
            continue;
        }

        // Handle the request
        printf("Before request:\nstored password: %s\nInput password: %s\n", password, inputpassword);
        handle_request(client_sock, request_buffer, ret, inputpassword);
        printf("After Request:\nstored password: %s\nInput password: %s\n", password, inputpassword);

        // Close the client socket
        close(client_sock);
    }

    // Close the server socket
    close(server_sock);

    return 0;
}

