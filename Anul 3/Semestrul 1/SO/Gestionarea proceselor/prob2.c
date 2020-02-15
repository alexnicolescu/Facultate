#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/wait.h>



int main()
{   

    int pidd;
    int pid;
    int status;
    for(int i=0;i<19;i++)
    {
        if((pid=fork())<0)
        {
            perror("Eroare la fork");
            exit(-1);
        }
        if(pid==0)
        {
            for(int j=0;j<10;j++)
                printf("fiu%d pid:%d,pid parinte:%d\n",i+1,getpid(),getppid());
            exit(i);
        }
    }
    printf("parinte pid:%d,pid parinte:%d\n",getpid(),getppid());
    for(int i=0;i<19;i++)
    {
        pidd=wait(&status);
        if(WIFEXITED(status))
        {
            printf("fiu%d,pid:%d,status:%d\n",i+1 ,pidd,WEXITSTATUS(status));
            
        }
    } 



    return 0;
}