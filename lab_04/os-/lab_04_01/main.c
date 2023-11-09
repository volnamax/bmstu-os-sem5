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

#define SE 0
#define SF 1
#define SB 2

struct sembuf producer_lock[2] = {{SE, -1, 0}, {SB, -1, 0}};
struct sembuf producer_release[2] = {{SB, 1, 0}, {SF, 1, 0}};

struct sembuf consumer_lock[2] = {{SF, -1, 0}, {SB, -1, 0}};
struct sembuf consumer_release[2] = {{SB, 1, 0}, {SE, 1, 0}};

int flag = 1;

void sig_handler(int sig_num)
{
    flag = 0;
    printf("Signal %d handled\n", sig_num);
}

int producer(char *addr, int semid)
{
    while (flag)
    {
        if (semop(semid, producer_lock, 2) == -1)
        {
            perror("semop");
            exit(1);
        }

        char **prod_cur = (char **)(addr + sizeof(char *));

        if (*prod_cur >= (addr + 42))
        {
            return 0;
        }

        char ch = 'a' + (*prod_cur - (addr + 2 * sizeof(char *))) % 26;

        **prod_cur = ch;

        (*prod_cur)++;

        printf("Producer %d write: %c\n", getpid(), ch);

        if (semop(semid, producer_release, 2) == -1)
        {
            perror("release");
            exit(1);
        }
    }

    return 0;
}

int consumer(char *addr, int semid)
{
    while (flag)
    {
        if (semop(semid, consumer_lock, 2) == -1)
        {
            perror("semop");
            exit(1);
        }

        char **cons_cur = (char **)addr;
        char c = **cons_cur;
        (*cons_cur)++;

        if (*cons_cur >= (addr + 42))
        {
            return 0;
        }

        printf("Consumer %d read: %c\n", getpid(), c);

        if (semop(semid, consumer_release, 2) == -1)
        {
            perror("release");
            exit(1);
        }
    }
    return 0;
}

int main()
{
    signal(SIGINT, sig_handler);

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
    int wstatus;
    for (short i = 0; i < 6; ++i)
    {
        pid_t w = wait(&wstatus);
        if (w == -1)
        {
            perror("wait error");
            return 1;
        }

        if (WIFEXITED(wstatus))
        {
            printf("exited, status=%d\n", WEXITSTATUS(wstatus));
        }
        else if (WIFSIGNALED(wstatus))
        {
            printf("killed by signal %d\n", WTERMSIG(wstatus));
        }
        else if (WIFSTOPPED(wstatus))
        {
            printf("stopped by signal %d\n", WSTOPSIG(wstatus));
        }
    }

    return 0;
}