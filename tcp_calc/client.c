#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 255

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    char buffer[SIZE];
    int num1, num2, choice, ans;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    while (1) {
        read(sockfd, buffer, SIZE); // Prompt for number 1
        printf("%s", buffer);
        scanf("%d", &num1);
        write(sockfd, &num1, sizeof(num1));

        read(sockfd, buffer, SIZE); // Prompt for number 2
        printf("%s", buffer);
        scanf("%d", &num2);
        write(sockfd, &num2, sizeof(num2));

        read(sockfd, buffer, SIZE); // Prompt for operation
        printf("%s", buffer);
        scanf("%d", &choice);
        write(sockfd, &choice, sizeof(choice));

        if (choice == 5) break; // Exit

        read(sockfd, &ans, sizeof(ans));
        printf("Answer: %d\n", ans);
    }

    close(sockfd);
    return 0;
}
