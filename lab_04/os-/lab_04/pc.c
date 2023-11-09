#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/signal.h>

#include <unistd.h>
#include "pc.h"

struct sembuf producer_lock[2] = {{SE, -1, 0}, {SB, -1, 0}};
struct sembuf producer_release[2] = {{SB, 1, 0}, {SF, 1, 0}};

struct sembuf consumer_lock[2] = {{SF, -1, 0}, {SB, -1, 0}};
struct sembuf consumer_release[2] = {{SB, 1, 0}, {SE, 1, 0}};



int producer(char *addr, int semid)
{
  while (1){
        if (semop(semid, producer_lock, 2) == -1)
        {
            perror("semop");
            exit(1);
        }

        char **prod_cur = (char **)(addr + sizeof(char *));
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
    while(1)
    {
        if (semop(semid, consumer_lock, 2) == -1)
        {
            perror("semop");
            exit(1);
        }

        char **cons_cur = (char **)addr;
        char c = **cons_cur;
        (*cons_cur)++;

        printf("Consumer %d read: %c\n", getpid(), c);

        if (semop(semid, consumer_release, 2) == -1)
        {
            perror("release");
            exit(1);
        }
    }
    return 0;
}
