#include "unp.h"

// maxline 4096

void str_cli(FILE *fp, int sockfd) {
    char sendline[MAXLINE], receline[MAXLINE];
    int n;
    while(fgets(sendline, MAXLINE, fp) != EOF) {
        write(sockfd, sendline, n);
        bzero(sendline, strlen(sendline));
        if (read(sockfd, receline, MAXLINE) == 0) {
            printf("str cli: server terminated prematurely");
	}
        fputs(receline,  stdout);
    }
}
