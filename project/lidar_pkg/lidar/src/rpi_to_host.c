// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <mqueue.h>
#include <poll.h>

#define PORT     8080
#define MAXLINE 1024

#define MQ_PATH "/measurements"

mqd_t measurement_queue = 0;

// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    mq_unlink(MQ_PATH);
    // Open queue
    measurement_queue = mq_open(MQ_PATH, O_CREAT | O_RDONLY | O_EXCL, 0700, NULL);
    if (measurement_queue < 0){
        fprintf(stderr, "Queue opening failed.\n");
        return EXIT_FAILURE;
    }

    // Setup pollfd
    struct pollfd poll_descriptor = {
        .fd = measurement_queue,
        .events = POLLIN
    };


    // Start UDP connection
    int len, n;
    len = sizeof(cliaddr);  //len is value/result
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    buffer[n] = '\0';
    printf("Connected.\n");

    while (1){
        int ready = poll(&poll_descriptor, 1, -1);
        if(ready < 0)
        {
            fprintf(stderr, "Poll failed.\n");
            return EXIT_FAILURE;
        }
        else if(poll_descriptor.revents & POLLIN){
            struct mq_attr attr;
            printf("Ready\n");
            mq_getattr(measurement_queue, &attr);
            char queue_buffer[attr.mq_msgsize];
            ssize_t received = mq_receive(measurement_queue, queue_buffer, attr.mq_msgsize, NULL);
            if (received < 0){
                fprintf(stderr, "mq receive failed.\n");
                return EXIT_FAILURE;
            }
            int sent = sendto(sockfd, (const char *)queue_buffer, strlen(queue_buffer),
                MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                    len);
            if (sent < 0){
                fprintf(stderr, "UDP send failed.\n");
                return EXIT_FAILURE;
            }
        }

    }

    return 0;
}