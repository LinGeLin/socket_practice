#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/select.h>

#define MAXLINE 4096
#define SERV_PORT 9877
#define LISTENQ 1024

void str_echo(int sockfd);
void err_sys(const char *, ...);


// client interface
void str_cli(FILE *fp, int sockfd);
