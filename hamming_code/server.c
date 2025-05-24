#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void generateHammingCode(int data[4], int hamming[7]) {
    // Place data bits
    hamming[2] = data[0];  // d1
    hamming[4] = data[1];  // d2
    hamming[5] = data[2];  // d3
    hamming[6] = data[3];  // d4

    // Calculate parity bits
    hamming[0] = hamming[2] ^ hamming[4] ^ hamming[6]; // p1
    hamming[1] = hamming[2] ^ hamming[5] ^ hamming[6]; // p2
    hamming[3] = hamming[4] ^ hamming[5] ^ hamming[6]; // p4
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int data[4], hamming[7];

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

    read(newsockfd, data, sizeof(data));
    printf("Received dataword from client: ");
    for (int i = 0; i < 4; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    generateHammingCode(data, hamming);
    printf("Generated Hamming Code: ");
    for (int i = 0; i < 7; i++) {
        printf("%d ", hamming[i]);
    }
    printf("\n");

    send(newsockfd, hamming, sizeof(hamming), 0);

    close(newsockfd);
    close(sockfd);
    return 0;
}
