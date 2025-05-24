#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_SIZE 255

void bit_stuffing(char *input, char *stuffed) {
    int i, j = 0, count = 0;
    int len = strlen(input);

    for (i = 0; i < len; i++) {
        if (input[i] == '1') {
            count++;
        } else {
            count = 0;
        }
        stuffed[j++] = input[i];
        if (count == 5) {
            stuffed[j++] = '0'; // insert stuffed 0
            count = 0;
        }
    }
    stuffed[j] = '\0';
}

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    char input[MAX_SIZE], stuffed[MAX_SIZE];

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

    printf("Enter binary data: ");
    scanf("%s", input);

    bit_stuffing(input, stuffed);
    printf("Bit Stuffed Data: %s\n", stuffed);

    send(sockfd, stuffed, strlen(stuffed), 0);
    printf("Stuffed Data Sent to Server.\n");

    close(sockfd);
    return 0;
}
