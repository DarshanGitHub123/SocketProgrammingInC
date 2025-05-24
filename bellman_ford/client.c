#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define INF 9999

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    int V, E, src;
    int u[100], v[100], w[100], dist[100];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        return 1;
    }

    printf("Enter number of vertices and edges: ");
    scanf("%d %d", &V, &E);
    printf("Enter each edge (u v w):\n");
    for (int i = 0; i < E; i++) {
        scanf("%d %d %d", &u[i], &v[i], &w[i]);
    }
    printf("Enter source vertex: ");
    scanf("%d", &src);

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

    write(sockfd, &V, sizeof(V));
    write(sockfd, &E, sizeof(E));
    write(sockfd, &src, sizeof(src));
    write(sockfd, u, sizeof(int) * E);
    write(sockfd, v, sizeof(int) * E);
    write(sockfd, w, sizeof(int) * E);

    read(sockfd, dist, sizeof(int) * V);

    printf("Shortest distances from vertex %d:\n", src);
    for (int i = 0; i < V; i++) {
        if (dist[i] == INF)
            printf("To %d: INF\n", i);
        else
            printf("To %d: %d\n", i, dist[i]);
    }

    close(sockfd);
    return 0;
}
