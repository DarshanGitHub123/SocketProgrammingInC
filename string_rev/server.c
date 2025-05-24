#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, n;
    socklen_t clilen;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;

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

    while (1) {
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if (n < 0) {
            perror("Error on reading");
            break;
        }

        if (strncmp("bye", buffer, 3) == 0)
            break;

        printf("String received: %s", buffer);

        int len = strlen(buffer);
        if (buffer[len - 1] == '\n') buffer[len - 1] = '\0'; // remove newline

        int i = 0, j = strlen(buffer) - 1;
        char temp;
        while (i < j) {
            temp = buffer[i];
            buffer[i] = buffer[j];
            buffer[j] = temp;
            i++;
            j--;
        }

        send(newsockfd, buffer, strlen(buffer), 0);
        printf("Reversed string sent to client\n");
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
