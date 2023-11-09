#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/signal.h>

#include "pc.h"

int main()
{

    int perms = S_IRWXU | S_IRWXG | S_IRWXO;

    int shm_fd = shmget(100, 2 * sizeof(char *) + 26 * sizeof(char), IPC_CREAT | perms);
    if (shm_fd == -1)
    {
        perror("shmget");
        exit(1);
    }

    char *addr = (char *)shmat(shm_fd, 0, 0);
    if (addr == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    char **cons_cur = (char **)addr;
    *cons_cur = addr + 2 * sizeof(char *);

    char **prod_cur = (char **)(addr + sizeof(char *));
    *prod_cur = addr + 2 * sizeof(char *);

    int sem_fd = semget(100, 3, IPC_CREAT | perms);
    if (sem_fd == -1)
    {
        perror("semget");
        exit(1);
    }

    semctl(sem_fd, SB, SETVAL, 1);
    semctl(sem_fd, SE, SETVAL, 26);
    semctl(sem_fd, SF, SETVAL, 0);

    pid_t child_pid;
    for (short i = 0; i < 3; ++i)
    {
        if ((child_pid = fork()) == -1)
        {
            perror("Can't fork.\n");
            return 1;
        }
        else if (child_pid == 0)
        {
            producer(addr, sem_fd);
            exit(0);
        }
    }

    for (short i = 0; i < 3; ++i)
    {
        if ((child_pid = fork()) == -1)
        {
            perror("Can't fork.\n");
            return 1;
        }
        else if (child_pid == 0)
        {
            consumer(addr, sem_fd);
            exit(0);
        }
    }

    return 0;
}
