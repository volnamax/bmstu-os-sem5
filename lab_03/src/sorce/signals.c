#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

//В программу с программным каналом включить собственный обработчик сигнала.
// Использовать сигнал для изменения хода выполнения программы.

// Глобальная переменная-флаг, изменяемая обработчиком сигнала
static int flag = 0;

void signal_handler(int sig_numb)
{
    flag = 1;
    printf("\nSignal %d received. Sending messages is allowed.\n", sig_numb);
}

int main(void)
{
    pid_t child_pid[2];
    char *messages[2] = {"aaaaaaaaaaaaaaaaaaa", \
                         "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"};
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Can't pipe\n");
        exit(EXIT_FAILURE);
    }
    // Установка обработчика сигнала SIGINT
    signal(SIGINT, signal_handler);
    printf("Press Ctrl+C to allow sending messages\n");
    sleep(2);

    for (size_t i = 0; i < 2; i++)
    {
        if ((child_pid[i] = fork()) == -1)
        {
            perror("Can't fork\n");
            exit(EXIT_FAILURE);
        }
        else if (child_pid[i] == 0)
        {
            printf("Child: pid = %d, ppid = %d, gid = %d\n", \
                getpid(), getppid(), getpgrp());
            if (flag)
            {
                printf("Child (pid: %d) sent message: %s\n", \
                    getpid(), messages[i]);
                close(fd[0]);
                write(fd[1], messages[i], strlen(messages[i]));
            }
            else
            {
                printf("Child (pid: %d) didn't send message\n", getpid());
            }

            return 0;
        }

        printf("Parent. Children id: %d\n", child_pid[i]);
    }

    for (size_t i = 0; i < 2; i++)
    {
        int status;
        waitpid(child_pid[i], &status, 0);

        if (WIFEXITED(status))
        {
            printf("Child (pid: %d) exited with code %d\n", \
                child_pid[i], WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Child (pid: %d) received signal %d\n", \
                child_pid[i], WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("Child (pid: %d) received signal %d\n", \
                child_pid[i], WSTOPSIG(status));
        }
    }

    printf("Messages from children: \n");
    // Вывод сообщений от дочерних процессов
    for (size_t i = 0; i < 2; i++)
    {
        char buf[64];
        close(fd[1]);
        read(fd[0], buf, strlen(messages[i]));
        printf("%s\n", buf);
    }

    return EXIT_SUCCESS;
}