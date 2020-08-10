#include    "unp.h"

int main(int argc, char **argv) {
    int i, maxi, maxfd, listenfd, connfd, sockfd;
	int nready, client[FD_SETSIZE];
	size_t n;
	fd_set rset, allset;
	char buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);

	maxfd = listenfd;
	maxi = -1;	// client 数组当前使用项的最大下标
	for(i=0; i<FD_SETSIZE; i++) {
		client[i] = -1;
	}
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

    for(;;) {
		rset = allset;
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		
		if(FD_ISSET(listenfd, &rset)) {
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);

			for (i=0; i<FD_SETSIZE; i++) {
				if (client[i] < 0 ) {
					client[i] = connfd;
					break;
				}
			}

			if (i == FD_SETSIZE) {
				exit(0);
			}

			FD_SET(connfd, &allset);
			if (connfd > maxfd) {
				maxfd = connfd;
			}

			if (i > maxi) {
				maxi = i;
			}

			if (--nready <= 0) {
				continue;
			}
		}

		for (i = 0; i <= maxi; i++) {
			if ((sockfd = client[i]) < 0) {
				continue;
			}

			if (FD_ISSET(sockfd, &rset)) {
				if ( ( n = read(sockfd, buf, MAXLINE)) == 0) {
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} else {
					write(sockfd, buf, n);
				}

				if (--nready <= 0) {
					break;
				}
			}
		}
    }
}
