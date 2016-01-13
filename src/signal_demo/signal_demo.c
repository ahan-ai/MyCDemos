#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "apueerror.h"

static void sig_int(int);

int main(int argc, char * argv[])
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        printf("signal error\n");
    }

    printf("%% ");
    while (fgets(buf, MAXLINE, stdin)) {
        if (buf[strlen(buf) - 1] == '\n') {
            buf[strlen(buf) - 1] = 0;
        }

        if ((pid = fork()) < 0) {
            printf("fork error\n");
        } else if (pid == 0) {
            execlp(buf, buf, (char*)0);
            err_ret("Couldn't execute: %s", buf);
            exit(127);
        }

        if ((pid = waitpid(pid, &status, 0)) < 0) {
            printf("waitpid error\n");
        }
        printf("%% ");
    }
    exit(0);
}

void sig_int(int signo) 
{
    printf("interrupt\n%% ");
    fflush(stdout);
}
