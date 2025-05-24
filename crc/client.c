#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 1024

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    char data[SIZE], divisor[SIZE], codeword[SIZE];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Input binary data and divisor
    printf("Enter binary data: ");
    scanf("%s", data);

    printf("Enter divisor (generator polynomial): ");
    scanf("%s", divisor);

    // Create socket
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send data and divisor
    send(sockfd, data, strlen(data) + 1, 0);
    send(sockfd, divisor, strlen(divisor) + 1, 0);

    // Receive codeword
    recv(sockfd, codeword, SIZE, 0);
    printf("Received Codeword from Server: %s\n", codeword);

    close(sockfd);
    return 0;
}
