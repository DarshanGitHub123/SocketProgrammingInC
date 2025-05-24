#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define FLAG 'F'
#define ESCAPE 'E'
#define MAX_SIZE 255

void byte_destuffing(const char *stuffed, char *destuffed) {
    int i = 0, j = 0;

    while (stuffed[i] != '\0') {
        if (stuffed[i] == ESCAPE) {
            i++;  // Skip escape and take the next character as-is
        }
        destuffed[j++] = stuffed[i++];
    }
    destuffed[j] = '\0';
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[MAX_SIZE], destuffed[MAX_SIZE];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    portno = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    int n = read(newsockfd, buffer, MAX_SIZE - 1);
    buffer[n] = '\0';
    printf("Received Stuffed Message: %s\n", buffer);

    // Remove start and end FLAGs
    if (buffer[0] == FLAG) {
        memmove(buffer, buffer + 1, strlen(buffer));  // remove start FLAG
    }
    if (buffer[strlen(buffer) - 1] == FLAG) {
        buffer[strlen(buffer) - 1] = '\0';  // remove end FLAG
    }

    byte_destuffing(buffer, destuffed);
    printf("Original Message after De-stuffing: %s\n", destuffed);

    close(newsockfd);
    close(sockfd);
    return 0;
}
