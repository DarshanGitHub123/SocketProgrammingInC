#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int sockfd, portno, words = 0;
    struct sockaddr_in serv_addr;
    char buffer[255], ch;
    FILE *f;

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

    f = fopen("sample.txt", "r");
    if (f == NULL) {
        perror("Error opening file");
        close(sockfd);
        return 1;
    }

    while ((ch = fgetc(f)) != EOF) {
        if (isspace(ch)) words++;
    }

    rewind(f);
    write(sockfd, &words, sizeof(int));

    char word[255];
    for (int i = 0; i < words; i++) {
        fscanf(f, "%s", word);
        write(sockfd, word, 255);
    }

    printf("The file has been sent successfully. Thank you.\n");
    fclose(f);
    close(sockfd);
    return 0;
}
