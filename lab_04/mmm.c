#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>

#define BUF_SIZE 10

//Идентификаторы семафоров
#define SE 0
#define SF 1
#define SB 2
//Количество итераций для каждого производителя/потребителя
#define ITERS_NUM 9
//Количество производителей и потребителей
#define NUM_PRODS 3
#define NUM_CONS 3
//todo del
#define N 10

typedef struct {
    char data[BUF_SIZE];
    int read_ptr;
    int write_ptr;
} buf_t;

struct sembuf consumer_p[2] = {{SF, -1, 0},
                               {SB, -1, 0}};
struct sembuf consumer_v[2] = {{SB, 1, 0},
                               {SE, 1, 0}};

struct sembuf producer_p[2] = {{SE, -1, 0},
                               {SB, -1, 0}};
struct sembuf producer_v[2] = {{SB, 1, 0},
                               {SF, 1, 0}};

int flag = 0;

void sig_handler(int signal) {
    flag = 1;
}

void init_buf(buf_t *buf) {
    buf->read_ptr = 0;
    buf->write_ptr = 0;
}

int write_to_buf(buf_t *buf, char symb) {
    buf->data[buf->write_ptr++] = symb;
    (buf->write_ptr) %= BUF_SIZE;
    return 0;
}

int read_from_buf(buf_t *buf, char *symb) {
    *symb = buf->data[buf->read_ptr++];
    (buf->read_ptr) %= BUF_SIZE;
    return 0;
}


void consumer(buf_t *const buf, const int sid, const int cid) {
    int sleep_time;
    while (flag != 1) {
        sleep_time = rand() % 3 + 1;
        sleep(sleep_time);

        if (semop(sid, consumer_p, 2) == -1) {
            perror("Something went wrong with consumer lock!");
            exit(1);
        }

        char ch;
        if (read_from_buf(buf, &ch) == -1) {
            perror("Can't read!");
            exit(1);
        }
        printf("Consumer[%d] №%d  read: %c (sleep: %d)\n", getpid(), cid, ch, sleep_time);
        if (semop(sid, consumer_v, 2) == -1) {
            perror("Something went wrong with consumer release!");
            exit(1);
        }
    }
}

void create_consumer(buf_t *const buffer, const int sid, const int cid) {
    pid_t childpid;
    if ((childpid = fork()) == -1) {
        perror("Can't fork!");
        exit(1);
    }

    if (childpid == 0) {
        for (int i = 0; i < ITERS_NUM; i++)
            consumer(buffer, sid, cid);
        exit(0);
    }
}
void producer(buf_t *const buf, char *ch, const int sid, const int pdid) {
    int sleep_time;
    while (flag != 1) {
        sleep_time = rand() % 3 + 1;
        sleep(sleep_time);

        if (semop(sid, producer_p, 2) == -1) {
            perror("producer_p!");
            exit(1);
        }

        (*ch)++;
        if (*ch > 122)
            *ch = 97;
        if (write_to_buf(buf, *ch) == -1) {
            perror("Can't write!");
            exit(1);
        }

        printf("Producer[%d] №%d write: %c (sleep: %d)\n", getpid(), pdid, (*ch), sleep_time);
        if (semop(sid, producer_v, 2) == -1) {
            perror("producer_v!");
            exit(1);
        }
    }
}

void create_producer(buf_t *const buf, char *const ch, const int sid, const int pdid) {
    pid_t childpid;
    if ((childpid = fork()) == -1) {
        perror("Can't fork!");
        exit(1);
    }

    if (childpid == 0) {
        for (int i = 0; i < ITERS_NUM; i++)
            producer(buf, ch, sid, pdid);
        exit(0);
    }
}

int main() {
    int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    // Создание разделяемой памяти
    int shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT | perms);
    if (shmid == -1) {
        perror("Failed to create shm!");
        exit(2);
    }
    //Присоединение разделяемой памяти к процессу
    buf_t *buffer;
    if ((buffer = (char *) shmat(shmid, 0, 0)) == (char *) -1) {
        perror("Shmat failed!");
        exit(2);
    }
    char *ch = (char *) (buffer + 1);
    *ch = 'a' - 1;
    init_buf(buffer);
    // создание семофоров
    int sem_descr = semget(IPC_PRIVATE, 3, IPC_CREAT | perms);
    if (sem_descr == -1)
        perror("Failed to create semaphores!");
    //Установка начальных значений семафоров
    if (semctl(sem_descr, SB, SETVAL, 1) == -1)
        perror("Can't set control bin_sem!");

    if (semctl(sem_descr, SF, SETVAL, 0) == -1)
        perror("Can't set control buf_full semaphore!");

    if (semctl(sem_descr, SE, SETVAL, N) == -1)
        perror("Can't set control buf_empty semaphor.");

    if (signal(SIGINT, sig_handler) == -1) {
        perror("Can't signal.\n");
        exit(1);
    }

    for (int i = 0; i < NUM_PRODS; i++)
        create_producer(buffer, ch, sem_descr, i + 1);

    for (int i = 0; i < NUM_CONS; i++)
        create_consumer(buffer, sem_descr, i + 1);

    for (size_t i = 0; i < NUM_PRODS + NUM_CONS; i++) {
        int status;
        if (wait(&status) == -1)
            perror("Something wrong with children waiting!");
        if (!WIFEXITED(status))
            printf("Children terminated abnormally!\n");
    }

    if (shmctl(shmid, IPC_RMID, NULL))
        perror("Failed to mark the segment to be destroyed!");


    if (shmdt((void *) buffer) == -1)
        perror("Failed to detachess the segment!");

    if (semctl(sem_descr, 0, IPC_RMID, 0) == -1)
        perror("Failed to delete semaphores!");


    return 0;
}
