#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX 255
#define BUCKET_SIZE 10
#define TOKEN_RATE 1

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[MAX];
    int tokens = 0;

    if (argc < 2) {
        fprintf(stderr, "Port number not provided\n");
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Binding failed");
        return 1;
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("Accept failed");
        return 1;
    }

    while (1) {
        if (tokens < BUCKET_SIZE)
            tokens += TOKEN_RATE;

        int n = read(newsockfd, buffer, MAX);
        if (n <= 0) break;
        buffer[n] = '\0';

        int packet = atoi(buffer);
        if (tokens >= packet) {
            tokens -= packet;
            printf("Sent packet %d, tokens left: %d\n", packet, tokens);
        } else {
            printf("Dropped packet %d, not enough tokens (have %d)\n", packet, tokens);
        }

        sleep(1);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
