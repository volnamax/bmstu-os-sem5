#include <stdio.h>
#include <unistd.h>

int main()
{
    int childpid, parentid;
    if ((childpid = fork()) == -1)
    {
        perror("cant fork");
        return 1;
    }
    else if (childpid == 0)
    {
        while (1)
        {
            printf("childpid =%d\n", getpid());
        }
        return 0;
        }
    else
    {
        while (1)
        {
            printf("parrentid = %d\n", getppid());
        }
        return 0;
    }
    return 0;
}