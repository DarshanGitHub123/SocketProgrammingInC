#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 255

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[SIZE];
    int num1, num2, choice, ans;

    if (argc < 2) {
        fprintf(stderr, "Port number not provided. Program terminated\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
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
        perror("Accept failed");
        exit(1);
    }

    while (1) {
        write(newsockfd, "Enter number 1: ", 16);
        read(newsockfd, &num1, sizeof(num1));

        write(newsockfd, "Enter number 2: ", 16);
        read(newsockfd, &num2, sizeof(num2));

        write(newsockfd, "1:Add 2:Sub 3:Mul 4:Div 5:Exit\n", 31);
        read(newsockfd, &choice, sizeof(choice));

        if (choice == 1) ans = num1 + num2;
        else if (choice == 2) ans = num1 - num2;
        else if (choice == 3) ans = num1 * num2;
        else if (choice == 4) ans = (num2 != 0) ? num1 / num2 : 0;
        else break;

        write(newsockfd, &ans, sizeof(ans));
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
