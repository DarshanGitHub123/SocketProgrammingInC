#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TOTAL_FRAMES 6
#define WINDOW_SIZE 3

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    int frames[TOTAL_FRAMES] = {0, 1, 2, 3, 4, 5};
    int ack[TOTAL_FRAMES] = {0};
    int frame, base = 0, i;

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

    while (base < TOTAL_FRAMES) {
        printf("\nSending window: ");
        for (i = base; i < base + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
            if (ack[i] == 0) {
                printf("%d ", frames[i]);
                send(sockfd, &frames[i], sizeof(frames[i]), 0);
            }
        }

        for (i = base; i < base + WINDOW_SIZE && i < TOTAL_FRAMES; i++) {
            if (ack[i] == 0) {
                if (recv(sockfd, &frame, sizeof(frame), 0) > 0) {
                    printf("\nReceived ACK for frame: %d", frame);
                    ack[frame] = 1;
                } else {
                    printf("\nACK not received for frame: %d", frames[i]);
                }
            }
        }

        while (ack[base] && base < TOTAL_FRAMES) base++;
    }

    // Send termination signal
    frame = -1;
    send(sockfd, &frame, sizeof(frame), 0);

    close(sockfd);
    return 0;
}
