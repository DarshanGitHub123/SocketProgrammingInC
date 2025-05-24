#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 255

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    char buffer[MAX];
    int packets[] = {1, 2, 4, 1, 3};
    int n = sizeof(packets) / sizeof(packets[0]);

    if (argc < 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        return 1;
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        sprintf(buffer, "%d", packets[i]);
        send(sockfd, buffer, strlen(buffer), 0);
        printf("Sent packet: %s\n", buffer);
        sleep(1);
    }

    close(sockfd);
    return 0;
}
