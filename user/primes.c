#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int fd)
{
    int prime = -1;
    read(fd, &prime, sizeof(int));
    printf("primes: %d\n", prime);

    int temp = -1;
    int p[2];
    pipe(p);

    while (1)
    {
        int n = read(fd, &temp, sizeof(int));
        if (n <= 0)
        {
            break;
        }

        if (temp % prime != 0)
        {
            write(p[1], &temp, sizeof(int));
        }
    }

    if (temp == -1)
    {
        close(p[1]);
        close(p[0]);
        close(fd);
        return;
    }

    int pid = fork();
    if (pid == 0)
    {
        close(p[1]);
        close(fd);
        primes(p[0]);
        close(p[0]);
    }
    else
    {
        close(p[1]);
        close(p[0]);
        close(fd);
        wait(0);
    }
}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    const int MAX = 101;
    for (int i = 2; i < MAX; i++)
    {
        write(p[1], &i, sizeof(int));
    }

    close(p[1]);
    primes(p[0]);
    close(p[0]);

    exit(0);
}
