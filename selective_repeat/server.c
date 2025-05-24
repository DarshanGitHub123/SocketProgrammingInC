#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, frame;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    if (argc < 2) {
        fprintf(stderr, "Port number not provided\n");
        return 1;
    }

    srand(time(NULL));  // seed random for simulating ACK loss

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
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
        recv(newsockfd, &frame, sizeof(frame), 0);
        if (frame == -1) break;

        printf("Received frame: %d\n", frame);

        if (rand() % 4 == 0) {
            printf("Simulated loss of ACK for frame: %d\n", frame);
            continue;
        }

        send(newsockfd, &frame, sizeof(frame), 0);
        printf("Sent ACK for frame: %d\n", frame);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
