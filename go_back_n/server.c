#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 1024

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    char buffer[MAX];
    int expected = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    printf("Server listening ...\n");

    while (1) {
        int len = recvfrom(sockfd, buffer, MAX, 0,
                           (struct sockaddr *)&cli_addr, &cli_len);
        buffer[len] = '\0';

        printf("Received: %s\n", buffer);

        int frame_no;
        sscanf(buffer, "Frame %d", &frame_no);

        if (frame_no == expected) {
            sprintf(buffer, "ACK %d", frame_no);
            sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&cli_addr, cli_len);
            printf("Sent: %s\n", buffer);
            expected++;
        } else {
            printf("Discarded: Frame %d\n", frame_no);
        }
    }

    close(sockfd);
    return 0;
}
