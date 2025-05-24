#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 1024

// XOR two binary strings of given length
void xor_operation(char *a, const char *b, int len) {
    for (int i = 0; i < len; i++) {
        a[i] = (a[i] == b[i]) ? '0' : '1';
    }
}

// Perform CRC division
void compute_crc(const char *data, const char *divisor, char *codeword) {
    int data_len = strlen(data);
    int div_len = strlen(divisor);
    char temp[SIZE];

    // Copy data and append zeros
    strcpy(temp, data);
    for (int i = 0; i < div_len - 1; i++) strcat(temp, "0");

    int total_len = data_len + div_len - 1;
    char remainder[SIZE];
    strncpy(remainder, temp, div_len);
    remainder[div_len] = '\0';

    for (int i = div_len; i <= total_len; i++) {
        if (remainder[0] == '1') {
            xor_operation(remainder, divisor, div_len);
        } else {
            char zero[SIZE];
            memset(zero, '0', div_len);
            zero[div_len] = '\0';
            xor_operation(remainder, zero, div_len);
        }

        // Shift left and append next bit
        memmove(remainder, remainder + 1, div_len - 1);
        remainder[div_len - 1] = temp[i];
        remainder[div_len] = '\0';
    }

    // Create final codeword = data + remainder
    strcpy(codeword, data);
    strcat(codeword, remainder);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char data[SIZE], divisor[SIZE], codeword[SIZE];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    portno = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive data and divisor
    recv(newsockfd, data, SIZE, 0);
    recv(newsockfd, divisor, SIZE, 0);

    compute_crc(data, divisor, codeword);
    printf("CRC Codeword computed: %s\n", codeword);

    send(newsockfd, codeword, strlen(codeword) + 1, 0);

    close(newsockfd);
    close(sockfd);
    return 0;
}
