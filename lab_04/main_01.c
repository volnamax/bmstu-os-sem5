#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/shm.h>

#include <unistd.h>

#define SE 0
#define SF 1
#define SB 2
// структуры которые описывают действия (операции) над процессами (семафо)
struct sembuf producer_lock[2] = {{SE, -1, 0},
                                  {SB, -1, 0}};
struct sembuf producer_release[2] = {{SB, 1, 0},
                                     {SF, 1, 0}};

struct sembuf consumer_lock[2] = {{SF, -1, 0},
                                  {SB, -1, 0}};
struct sembuf consumer_release[2] = {{SB, 1, 0},
                                     {SE, 1, 0}};


int flag = 0;

void sig_handler(int signal) {
    flag = 1;
}


int producer(char *addr, int semid) {
    int sleep_time;
    while (flag != 1) {
        sleep_time = rand() % 3 + 1;
        sleep(sleep_time);
        if (semop(semid, producer_lock, 2) == -1) {
            perror("semop");
            exit(1);
        }

        char **prod_cur = (char **) (addr + sizeof(char *));
        char ch = 'a' + (*prod_cur - (addr + 2 * sizeof(char *))) % 26;

        if (ch == 'z')
            break;

        **prod_cur = ch;

        (*prod_cur)++;

        printf("Producer %d write: %c\n", getpid(), ch);

        if (semop(semid, producer_release, 2) == -1) {
            perror("release");
            exit(1);
        }
    }


    return 0;
}

int consumer(char *addr, int semid) {
    int sleep_time;
    while (flag != 1) {
        sleep_time = rand() % 2 + 1;
        sleep(sleep_time);

        if (semop(semid, consumer_lock, 2) == -1) {
            perror("semop");
            exit(1);
        }

        char **cons_cur = (char **) addr;
        char c = **cons_cur;
        (*cons_cur)++;

        printf("Consumer %d read: %c\n", getpid(), c);

        if (semop(semid, consumer_release, 2) == -1) {
            perror("release");
            exit(1);
        }
    }
    return 0;
}


int main() {
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        perror("Can't signal.\n");
        exit(1);
    }

    int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    int shm_fd = shmget(IPC_PRIVATE, 2 * sizeof(char *) + 26 * sizeof(char), IPC_CREAT | perms);
    if (shm_fd == -1) {
        perror("shmget");
        exit(1);
    }

    char *addr = (char *) shmat(shm_fd, 0, 0);
    if (addr == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    char **cons_cur = (char **) addr;
    *cons_cur = addr + 2 * sizeof(char *);

    char **prod_cur = (char **) (addr + sizeof(char *));
    *prod_cur = addr + 2 * sizeof(char *);

    int sem_fd = semget(IPC_PRIVATE, 3, IPC_CREAT | perms);
    if (sem_fd == -1) {
        perror("semget");
        exit(1);
    }

    if (semctl(sem_fd, SB, SETVAL, 1)) {
        perror("err bin sem");
    }
    if (semctl(sem_fd, SE, SETVAL, 26)) {
        perror("err control buf empty");
    }
    if (semctl(sem_fd, SF, SETVAL, 0)) {
        perror("err buf full");
    }

    pid_t child_pid;

    for (short i = 0; i < 3; ++i) {
        if ((child_pid = fork()) == -1) {
            perror("Can't fork.\n");
            return 1;
        } else if (child_pid == 0) {
            consumer(addr, sem_fd);
            exit(0);
        }
    }

    for (short i = 0; i < 3; ++i) {
        if ((child_pid = fork()) == -1) {
            perror("Can't fork.\n");
            return 1;
        } else if (child_pid == 0) {
            producer(addr, sem_fd);
            exit(0);
        }
    }


    int wstatus;
    for (short i = 0; i < 6; ++i) {
        pid_t w = wait(&wstatus);
        if (w == -1) {
            perror("wait error");
            return 1;
        }
        if (WIFEXITED(wstatus)) {
            printf("exited, status=%d\n", WEXITSTATUS(wstatus));
        } else if (WIFSIGNALED(wstatus)) {
            printf("killed by signal %d\n", WTERMSIG(wstatus));
        } else if (WIFSTOPPED(wstatus)) {
            printf("stopped by signal %d\n", WSTOPSIG(wstatus));
        }
    }
    if (shmctl(shm_fd, IPC_RMID, NULL))
        perror("Failed to mark the segment to be destroyed!");

    if (shmdt((void *) addr) == -1)
        perror("Failed to detachess the segment!");

    if (semctl(sem_fd, 0, IPC_RMID, 0) == -1)
        perror("Failed to delete semaphores!");

    return 0;
}
