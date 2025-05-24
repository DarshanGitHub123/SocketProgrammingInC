#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define FLAG 'F'
#define ESCAPE 'E'
#define MAX_SIZE 255

void byte_stuffing(const char *input, char *stuffed) {
    int i, j = 0;
    stuffed[j++] = FLAG;  // Start flag

    for (i = 0; input[i] != '\0'; i++) {
        if (input[i] == FLAG || input[i] == ESCAPE) {
            stuffed[j++] = ESCAPE;  // Insert escape before FLAG or ESCAPE
        }
        stuffed[j++] = input[i];
    }

    stuffed[j++] = FLAG;  // End flag
    stuffed[j] = '\0';
}

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    char input[MAX_SIZE], stuffed[MAX_SIZE];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

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

    printf("Enter message: ");
    scanf(" %[^\n]", input);  // Read full line including spaces

    byte_stuffing(input, stuffed);
    printf("Stuffed Message: %s\n", stuffed);

    send(sockfd, stuffed, strlen(stuffed), 0);
    printf("Stuffed data sent to server.\n");

    close(sockfd);
    return 0;
}
