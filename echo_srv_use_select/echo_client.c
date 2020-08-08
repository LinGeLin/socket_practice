#include    "unp.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    int sockfd[5];
    struct sockaddr_in servaddr;
    int i=0;
    for (i=0; i<5; i++) {
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);
        inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    
        connect(sockfd[i], (struct sockaddr*)&servaddr, sizeof(servaddr));
    }
    
    str_cli(stdin, sockfd[0]);
    exit(0);    
}
