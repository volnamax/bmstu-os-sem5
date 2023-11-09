//
// Created by max on 06.10.23.
//Написать программу, в которой предок и потомок обмениваются сообщением через программный канал.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    // Массив для хранения идентификаторов дочерних процессов
    pid_t child_pid[2];

    // Массив с сообщениями, которые будут отправлены из дочерних процессов
    char *messages[2] = {"aaaaaaaaaaaaaaaaaaa", \
                         "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"};

    // Массив для хранения файловых дескрипторов канала
    int fd[2];

    // Создание канала
    if (pipe(fd) == -1)
    {
        perror("Can't pipe\n");
        exit(1);
    }

    // Создание двух дочерних процессов
    for (size_t i = 0; i < 2; i++)
    {
        if ((child_pid[i] = fork()) == -1)
        {
            // В случае ошибки при создании дочернего процесса
            perror("Can't fork\n");
            exit(1);
        }
        else if (child_pid[i] == 0)
        {
            // Код, выполняемый в дочернем процессе

            // Вывод информации о дочернем процессе
            printf("Child: pid = %d, ppid = %d, gid = %d\n", \
                getpid(), getppid(), getpgrp());

            // Вывод сообщения, которое будет отправлено
            printf("Child (pid: %d) sent message: %s\n", \
                getpid(), messages[i]);

            // Закрытие ненужного конечного файла канала
            close(fd[0]);

            // Запись сообщения в канал
            write(fd[1], messages[i], strlen(messages[i]));

            // Завершение работы дочернего процесса
            return 0;
        }

        // Код, выполняемый в родительском процессе
        printf("Parent. Children id: %d\n", child_pid[i]);
    }

    // Ожидание завершения дочерних процессов
    for (size_t i = 0; i < 2; i++)
    {
        int status;
        waitpid(child_pid[i], &status, 0);

        // Обработка статуса завершения дочернего процесса
        if (WIFEXITED(status))
        {
            // Если дочерний процесс завершился нормально
            printf("Child (pid: %d) exited with code %d\n", \
                child_pid[i], WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            // Если дочерний процесс завершился из-за сигнала
            printf("Child (pid: %d) received signal %d\n", \
                child_pid[i], WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            // Если дочерний процесс был остановлен сигналом
            printf("Child (pid: %d) received signal %d\n", \
                child_pid[i], WSTOPSIG(status));
        }
    }

    // Вывод сообщений от дочерних процессов
    printf("Messages from children: \n");

    for (size_t i = 0; i < 2; i++)
    {
        char buf[64];

        // Закрытие ненужного конечного файла канала
        close(fd[1]);

        // Чтение сообщения из канала
        read(fd[0], buf, strlen(messages[i]));

        // Вывод сообщения
        printf("%s\n", buf);
    }

    return 0;
}




//
//// оба child пишут в pipe ()\n
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/wait.h>
//#include <unistd.h>
//
//int
//main(int argc, char *argv[])
//{
//    int    pipefd[2];
//    char   buf;
//    pid_t  cpid;
//
//    if (argc != 2) {
//        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
//        exit(EXIT_FAILURE);
//    }
//
//    if (pipe(pipefd) == -1) {
//        perror("pipe");
//        exit(EXIT_FAILURE);
//    }
//
//    cpid = fork();
//    if (cpid == -1) {
//        perror("fork");
//        exit(EXIT_FAILURE);
//    }
//
//    if (cpid == 0) {    /* Child reads from pipe */
//        close(pipefd[1]);          /* Close unused write end */
//
//        while (read(pipefd[0], &buf, 1) > 0)
//            write(STDOUT_FILENO, &buf, 1);
//
//        write(STDOUT_FILENO, "\n", 1);
//        close(pipefd[0]); // тип не надо так как кто читает тсам закроет
//        _exit(EXIT_SUCCESS);
//
//    } else {            /* Parent writes argv[1] to pipe */
//        close(pipefd[0]);          /* Close unused read end */
//        write(pipefd[1], argv[1], strlen(argv[1]));
//        close(pipefd[1]);          /* Reader will see EOF */
//        wait(NULL);                /* Wait for child */
//        exit(EXIT_SUCCESS);
//    }
//}
