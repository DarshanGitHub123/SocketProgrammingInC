#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    int data[4], hamming[7];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Enter a 4-bit binary dataword (space-separated): ");
    for (int i = 0; i < 4; i++) {
        scanf("%d", &data[i]);
    }

    send(sockfd, data, sizeof(data), 0);
    read(sockfd, hamming, sizeof(hamming));

    printf("Received Hamming Code from server: ");
    for (int i = 0; i < 7; i++) {
        printf("%d ", hamming[i]);
    }
    printf("\n");

    close(sockfd);
    return 0;
}
