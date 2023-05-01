// Server side C program to demonstrate HTTP Server programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 80
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char usergivenusername[50];
    char usergivenpassword[50];
    char username[50] = "dhanush";
    char password[50] = "password";
    printf("username: %s password: %s", username, password);


    // Only this line has been changed. Everything is same.
//    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body></body></html>";
    char *hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Login page</h1><form method=\"post\"><label for=\"username\">Username:</label><input type=\"text\" id=\"username\" name=\"username\"><br><label for=\"password\">Password:</label><input type=\"password\" id=\"password\" name=\"password\"><br><input type=\"submit\" value=\"Submit\"></form></body></html>\r\n";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    memset(address.sin_zero, '\0', sizeof address.sin_zero);


    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        char method[5]; // assume the method type is at most 4 characters long
        sscanf(buffer, "%s", method);

        if (strcmp(method, "GET") == 0) {
            // Handle GET request
            printf("GET\n");
        } else if (strcmp(method, "POST") == 0) {
            // Handle POST request
            printf("POST\n");
            char *content_length_str = strstr(buffer, "Content-Length:");
            if (content_length_str != NULL) {
                int content_length;
                sscanf(content_length_str, "Content-Length: %d", &content_length);

                char *data_start = strstr(buffer, "\r\n\r\n") + 4; // skip headers
                if (strlen(data_start) >= content_length) {
                    char data[content_length+1];
                    strncpy(data, data_start, content_length);
                    data[content_length] = '\0';

                    // Now the data variable contains the POST data
                    printf("POST data: %s\n", data);
                    char *key_value_pair = strtok(data, "&");
                    while (key_value_pair != NULL) {
                        char key[256], value[256];
                        sscanf(key_value_pair, "%[^=]=%s", key, value);
                        if (strcmp(key, "username") == 0) {
                            strcpy(usergivenusername, value);
                        } else if (strcmp(key, "password") == 0) {
                            strcpy(usergivenpassword, value);
                        }

                        key_value_pair = strtok(NULL, "&");
                    }
                    printf("%s\n%s\n\n", usergivenusername, usergivenpassword);
                    printf("username: %s password: %s\n\n", username, password);
               }
            }
        }
        printf("%s\n",buffer );
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------");
        close(new_socket);
    }
    return 0;
}

//  to overflow the buffer:
// username = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" => "A"*100
// password = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB" => "B"*100
// 150 * "B" to overwrite the password
