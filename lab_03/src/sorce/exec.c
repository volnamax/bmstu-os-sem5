#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Написать программу, в которой процесс-потомок вызывает си
// стемный вызов exec(), а процесс-предок ждет завершения процесса-потомка. Следует создать не менее двух потомков.
// запустить с помощью exec() программу в процессе потомке, процесс потомок ждет

int main(void)
{
    pid_t child_pid[2];
    char *commands[2] = {"./lab_04_03_00/app.exe", "./lab_10_02_04/app.exe"};
    char *args1[] = {"./lab_04_03_00/app.exe", NULL};
    char *args2[] = {"./lab_10_02_04/app.exe",  NULL};
    char **args[] = {args1, args2};

    // Создание двух дочерних процессов
    for (size_t i = 0; i < 2; i++)
    {
        if ((child_pid[i] = fork()) == -1)
        {
            perror("Can't fork\n");
            exit(1);
        }
        else if (child_pid[i] == 0)
        {
            // Код, выполняемый в дочернем процессе

            printf("Child: pid = %d, ppid = %d, gid = %d\n", \
                getpid(), getppid(), getpgrp());

            // Запуск программы в дочернем процессе
            if (execvp(commands[i], args[i])  == -1)
            {
                perror("Can't exec\n");
                exit(1);
            }
            // Код после execvp() не выполняется, так как процесс заменен новым
            return 0;
        }

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

    return 0;
}

