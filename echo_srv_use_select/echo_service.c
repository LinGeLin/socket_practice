#include    "unp.h"

void sig_chld(int signo) {
    pid_t pid;
    int stat;
    // 通过stat指针返回子进程终止状态
    // 正常终止、信号杀死、作业控制停止
    // pid = wait(&stat);
    
    // 防止同一信号多次提交导致子进程僵死
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
	// do nothing
    }
}

int main(int argc, char **argv) {
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);
    signal(SIGCHLD, sig_chld);
    for(;;) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        if ((childpid = fork()) == 0) {
            close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        close(connfd);
    }
}
