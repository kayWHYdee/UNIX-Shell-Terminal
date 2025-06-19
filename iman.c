#include "headers.h"
#define URL "man.he.net"
#define PORT "80"
void print_clean(const char *response) {
    int in_tag = 0; // Keeps track if we are inside an HTML tag

    for (int i = 0; response[i] != '\0'; i++) {
        if (response[i] == '<') {
            in_tag = 1; // We've encountered the start of an HTML tag
        } else if (response[i] == '>') {
            in_tag = 0; // We've encountered the end of an HTML tag
        } else if (!in_tag) {
            // Print the character only if it's not inside a tag
            putchar(response[i]);
        }
    }
}
void iman_command(char *command) {
    char path[256] = {0};
    char *token = strtok((char *)command, " ");
    
    token = strtok(NULL, " ");
    if (token == NULL) {
        fprintf(stderr, "Error: No command specified for iMan\n");
        return;
    }

    strcpy(path, "/?topic=");
    strcat(path, token);
    strcat(path, "&section=all");
    // printf("Path is %s\n", path);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *result;
    int status = getaddrinfo(URL, PORT, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        fprintf(stderr, "Failed to resolve host name!\n");
        return;
    }

    int fd = -1;
    for (struct addrinfo *r = result; r != NULL; r = r->ai_next) {
        int cur = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
        if (cur == -1)
            continue;

        if (connect(cur, r->ai_addr, r->ai_addrlen) != -1) {
            fd = cur;
            break;
        }
        close(cur);
    }
    freeaddrinfo(result);
    if (fd == -1) {
        fprintf(stderr, "Failed to connect to man.he.net\n");
        return;
    }

    char request[MAX_LENGTH] = {0};
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, URL);
    
    if (send(fd, request, strlen(request), 0) == -1) {
        fprintf(stderr, "send failed with errno %i (%s)\n", errno, strerror(errno));
        fprintf(stderr, "Failed to send request\n");
        close(fd);
        return;
    }
    char response[MAX_LENGTH] = {0};
    ssize_t bytes_received;
    int first = 1;

    while ((bytes_received = recv(fd, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0';

        if (first) {
            first = 0;
            char *cur = strstr(response, "NAME");
            if (cur == NULL) {
                fprintf(stderr, "No such command found\n");
                close(fd);
                return;
            }
            print_clean(cur);
        } else {
            print_clean(response);
        }
    }

    if (bytes_received == -1) {
        fprintf(stderr, "recv failed with errno %i (%s)\n", errno, strerror(errno));
    }

    close(fd);
    return;
}