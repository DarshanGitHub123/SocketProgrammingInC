#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 512

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, n;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[SIZE], filename[SIZE];

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

    bzero(filename, SIZE);
    read(newsockfd, filename, SIZE);
    printf("Client requested: %s\n", filename);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        char *msg = "File not found.";
        write(newsockfd, msg, strlen(msg));
    } else {
        while (fgets(buffer, SIZE, fp) != NULL) {
            write(newsockfd, buffer, strlen(buffer));
        }
        printf("File sent successfully.\n");
        fclose(fp);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
