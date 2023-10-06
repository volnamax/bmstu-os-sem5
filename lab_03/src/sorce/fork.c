#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Написать программу, запускающую не мене двух новых процессов системным вызовом fork().
// В предке вывести собственный идентификатор (функция getpid()), идентификатор группы ( функция getpgrp())
// и идентификаторы потомков. В процессе-потомке вывести собственный идентификатор,
// идентификатор предка (функция getppid()) и идентификатор группы. Убедиться,
// что при завершении процесса-предка потомок, который продолжает выполняться,
// получает идентификатор предка (PPID), равный 1 или идентификатор процесса-посредника
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
            // Код, выполняемый в новом дочернем процессе
        else if (child_pid[i] == 0)
        {
            printf("Child: pid = %d, ppid = %d, gid = %d\n", \
                getpid(), getppid(), getpgrp());
            sleep(2);
            printf("Child: pid = %d, ppid = %d, gid = %d\n", \
                getpid(), getppid(), getpgrp());
            // Завершение работы дочернего процесса
            return 0;
        }

        printf("Parent. Children id: %d\n", child_pid[i]);
    }

    return 0;
}
