#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_SIZE 255

void bit_destuffing(char *stuffed, char *destuffed) {
    int i, j = 0, count = 0;
    int len = strlen(stuffed);

    for (i = 0; i < len; i++) {
        if (stuffed[i] == '1') {
            count++;
        } else {
            count = 0;
        }
        destuffed[j++] = stuffed[i];

        // If 5 consecutive 1s and next bit is 0, skip it
        if (count == 5 && stuffed[i + 1] == '0') {
            i++; // skip stuffed 0
            count = 0;
        }
    }
    destuffed[j] = '\0';
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[MAX_SIZE], destuffed[MAX_SIZE];

    if (argc < 2) {
        fprintf(stderr, "Port number not provided. Program terminated\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("Error on accept");
        exit(1);
    }

    bzero(buffer, MAX_SIZE);
    read(newsockfd, buffer, MAX_SIZE);

    printf("Received Stuffed Data: %s\n", buffer);

    bit_destuffing(buffer, destuffed);

    printf("Original Data after De-stuffing: %s\n", destuffed);

    close(newsockfd);
    close(sockfd);
    return 0;
}
