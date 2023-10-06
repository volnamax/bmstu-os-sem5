//
// Created by max on 05.10.23.
//Задание 2: написать программу по схеме первого задания,
// но в процессе-предке выполнить системный вызов wait().
// Убедиться, что в этом случае идентификатор процесса потомка на 1
// больше идентификатора процесса-предка.
//


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t child_pid[2];

    for (size_t i = 0; i < 2; i++)
    {
        if ((child_pid[i] = fork()) == -1)
        {
            perror("Can't fork\n");
            exit(1);
        }
        else if (child_pid[i] == 0)
        {
            printf("Child: pid = %d, ppid = %d, gid = %d\n", \
                getpid(), getppid(), getpgrp());
            return 0;
        }

        printf("Parent. Children id: %d\n", child_pid[i]);
    }
    // Ожидание завершения дочерних процессов
    for (size_t i = 0; i < 2; i++)
    {
        int status;
        waitpid(child_pid[i], &status, 0);

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

    return 0;
}