#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>

static void getShellInfo(char *shell_info) {
    // 获取用户名
    struct passwd *pwd;
    pwd = getpwuid(getuid());

    // 获取主机名
    char hostname[64];
    gethostname(hostname, sizeof(hostname));

    // 获取当前工作路径
    char path_name[300];
    getcwd(path_name, 300);
    if (strstr(path_name, pwd->pw_dir)) {
        int home_path_len = strlen(pwd->pw_dir);
        int path_len = strlen(path_name);
        path_name[0] = '~';
        int i = 1;
        for(; i<path_len-home_path_len+1; i++) {
            path_name[i] = path_name[i + home_path_len - 1];
        }
        for (; i<path_len; i++) {
            path_name[i] = '\0';
        }
    }
    
    char *at = {"@"};
    char *colon = {":"};
    char *dollar = {"$ "};
    char *hash = {"# "};

    strcat(shell_info, pwd->pw_name);
    strcat(shell_info, at);
    strcat(shell_info, hostname);
    strcat(shell_info, colon);
    strcat(shell_info, path_name);
    if (strcmp(pwd->pw_name, "root") == 0) {
        strcat(shell_info, hash);
    } else {
        strcat(shell_info, dollar);
    }
}

int main() {
    int st = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == st) {
        perror("Socket create error: ");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8080);

    if (-1 == bind(st, (struct sockaddr*)&addr, sizeof(addr))) {
        perror("Bind addr error: ");
        return EXIT_FAILURE;
    }

    if (-1 == listen(st, 20)) {
        perror("Listen error: ");
        return EXIT_FAILURE;
    }

    struct sockaddr_in client_addr;
    socklen_t len;
    int client_st = 0;
    char cmd_line[256];
    char shell_info[512];
    while(1) {
        memset(&client_addr, 0, sizeof(client_addr));

        len = sizeof(client_addr);
        client_st = accept(st, (struct sockaddr*)&client_addr, &len);
        if (-1 == client_st) {
            perror("Accept error: ");
            return EXIT_FAILURE;
        }

        while(1) {
            memset(shell_info, 0, sizeof(shell_info));
            getShellInfo(shell_info);
            if (-1 == write(client_st, shell_info, strlen(shell_info))) {
                perror("Write error: ");
                return EXIT_FAILURE;
            }
            memset(cmd_line, 0, sizeof(cmd_line));
            int rv = recv(client_st, cmd_line, sizeof(cmd_line), 0);
            if (rv > 0) {
                printf("%s\n", cmd_line);
            } else if (rv == 0) {
                break;
            } else {
                perror("Recv error: ");
                return EXIT_FAILURE;
            }
        }
        close(client_st);
    }
    close(st);
    return EXIT_SUCCESS;
}