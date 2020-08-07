#include    "unp.h"
/*
void str_echo(int sockfd) {
    ssize_t n;
    char buf[MAXLINE];

    while( (n = read(sockfd, buf, MAXLINE)) > 0) {
        write(sockfd, buf, n);
    }
}
*/

void str_echo(int sockfd) {
    long arg1, arg2;
    ssize_t n;
    char line[MAXLINE];

    for(;;) {
	if ( (n = read(sockfd, line, MAXLINE)) == 0) {
	    return;
	}
	if (sscanf(line, "%ld%ld", &arg1, &arg2) == 2) {
	    snprintf(line, sizeof(line), "%ld\n", arg1, arg2);
	}
	else {
	    snprintf(line, sizeof(line), "input error\n");
	}
        n = strlen(line);
	write(sockfd, line, n);
    }
}
