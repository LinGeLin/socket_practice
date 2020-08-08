#include "unp.h"

// maxline 4096

int max(int a, int b) {
    return a>b? a : b;
}

void str_cli(FILE *fp, int sockfd) {
	int maxfdp1, stdineof;
    fd_set rset;
    char sendline[MAXLINE], receline[MAXLINE];
    int n;

    stdineof = 0;
    FD_ZERO(&rset);
    for (; ;) {
        // fileno 将标准 I/O 文件指针转换为对应的描述符
        if (stdineof == 0) {
            FD_SET(fileno(fp), &rset);
		}
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		select(maxfdp1, &rset, NULL, NULL, NULL);

		// 如果服务挂掉，客户端可以接受到消息，并退出
		if (FD_ISSET(sockfd, &rset)) { // socket is readable
		    if ((n = read(sockfd, receline, MAXLINE)) == 0) {
				if (stdineof == 1) {
				    return;
				} else {
			 	   printf("str_cli: server terminated prematurely");
				   return;
				}
			}
       		write(fileno(stdout), receline, strlen(receline));
			bzero(receline, strlen(receline));
		}

		if (FD_ISSET(fileno(fp), &rset)) {
			if ( (n = read(fileno(stdin), sendline, MAXLINE) == 0) ){
				stdineof = 1;
				shutdown(sockfd, SHUT_WR);
				FD_CLR(fileno(fp), &rset);
				continue;
			}
	    	write(sockfd, sendline, strlen(sendline));
	    	bzero(sendline, strlen(sendline));
		}
	}
}
