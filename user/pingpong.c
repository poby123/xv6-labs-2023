#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 20

int main(int argc, char *argv[])
{
    int fd1[2], fd2[2], pid;
    char recv_message[MAX] = {0};

    if (pipe(fd1) < 0)
    {
        printf("Pipe error is occured.\n");
        exit(1);
    }

    if (pipe(fd2) < 0)
    {
        printf("Pipe error is occured.\n");
        exit(1);
    }

    pid = fork();

    // children process
    if (pid == 0)
    {
        char send_message[MAX] = "ping";

        write(fd1[1], send_message, sizeof(send_message));
        read(fd2[0], recv_message, sizeof(recv_message));

        printf("%d: received %s\n", getpid(), recv_message);
    }

    // parent process
    else
    {
        char send_message[MAX] = "pong";

        read(fd1[0], recv_message, sizeof(recv_message));
        printf("%d: received %s\n", getpid(), recv_message);

        write(fd2[1], send_message, sizeof(send_message));
        sleep(2);
    }

    exit(0);
}
