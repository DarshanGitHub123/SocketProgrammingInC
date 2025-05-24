#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 1024
#define WINDOW_SIZE 4
#define TIMEOUT 2

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    socklen_t addr_len = sizeof(serv_addr);
    char buffer[MAX];
    int base = 0, next_frame = 0, total_frames, ack;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s server_ip port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    printf("Enter number of frames to send: ");
    scanf("%d", &total_frames);

    while (base < total_frames) {
        while (next_frame < base + WINDOW_SIZE && next_frame < total_frames) {
            sprintf(buffer, "Frame %d", next_frame);
            sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&serv_addr, addr_len);
            printf("Sent: %s\n", buffer);
            next_frame++;
        }

        sleep(TIMEOUT); // simulate delay

        while (recvfrom(sockfd, buffer, MAX, MSG_DONTWAIT,
                        (struct sockaddr *)&serv_addr, &addr_len) > 0) {
            sscanf(buffer, "ACK %d", &ack);
            printf("Received: ACK %d\n", ack);
            if (ack >= base) base = ack + 1;
        }

        // retransmit if necessary
        if (base < next_frame) {
            next_frame = base;
        }
    }

    printf("All frames sent successfully!\n");
    close(sockfd);
    return 0;
}
