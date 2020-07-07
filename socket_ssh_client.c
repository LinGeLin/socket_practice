#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int SSHPORT = 8080;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s user@ip\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *ip = strchr(argv[1], '@') + 1;
    if (strlen(ip) > 15) {
        printf("Wrong ip: %s\n", ip);
        return EXIT_FAILURE;
    }
    
    int st = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == st) {
        printf("Create socket fail: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SSHPORT);

    if (-1 == connect(st, (struct sockaddr*)&addr, sizeof(addr))) {
        printf("connect failed, %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    char cmd_line[1024];
    char result[10240];
    while(1) {
        if (-1 == read(st, result, sizeof(result))) {
            perror("Read error: ");
            return EXIT_FAILURE;
        } else {
            printf("%s", result);
        }
        memset(cmd_line, 0, sizeof(cmd_line));
        gets(cmd_line);
        if (strcmp(cmd_line, "exit") == 0) {
            break;
        }
        if (-1 == send(st, cmd_line, strlen(cmd_line), 0)) {
            printf("send command to server fail. %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
    }
    close(st);
    return EXIT_SUCCESS;
}