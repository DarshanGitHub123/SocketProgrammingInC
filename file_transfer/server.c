#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, words, count = 0;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[255];
    FILE *fp;

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

    fp = fopen("sample_received.txt", "w");
    if (fp == NULL) {
        perror("File open error");
        close(newsockfd);
        close(sockfd);
        return 1;
    }

    read(newsockfd, &words, sizeof(int));

    while (count < words) {
        bzero(buffer, 255);
        read(newsockfd, buffer, 255);
        fprintf(fp, "%s ", buffer);
        count++;
    }

    printf("File has been received successfully. It is saved as sample_received.txt\n");

    fclose(fp);
    close(newsockfd);
    close(sockfd);
    return 0;
}
