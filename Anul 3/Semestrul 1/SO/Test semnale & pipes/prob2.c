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
#include <signal.h>
#include <time.h>

int errorSignal=-1;
int fdes[2];
int contor=0;
void copil1(char *cale)
{
    close(fdes[0]);
    int dupVal;
    dupVal=dup2(fdes[1],1);
    if(dupVal<0)
    {
        perror("Eroare la functia dup2");
        exit(errorSignal--);
    }
    execlp("cat","cat",cale,NULL);
    close(fdes[1]);
    perror("Eroare la exec");
    exit(errorSignal--);

}
void copil2(char *cale)
{
    close(fdes[0]);
    int dupVal;
    dupVal=dup2(fdes[1],1);
    if(dupVal<0)
    {
        perror("Eroare la functia dup2");
        exit(errorSignal--);
    }
    execlp("cat","cat",cale,NULL);
    close(fdes[1]);
    perror("Eroare la exec");
    exit(errorSignal--);

}
void copil3(void)
{
    close(fdes[1]);
    char buff[4096];
    int n,nr=0;
    while((n=read(fdes[0],(void *)buff,4096))>0)
    {
        for(int i=0;i<n;i++)
            if(isdigit(buff[i]))
            {
                if(kill(getppid(),SIGUSR1)<0)
                {
                    perror("Nu se poate trimite semnalul SIGUSR1 la parinte");
                    exit(errorSignal--);
                }
                nr++;

            }
        }
        close(fdes[0]);
        if(n==-1)
        {
            perror("Nu se poate citi din pipe");
            exit(errorSignal--);
        }
        exit(nr);
    }
    void handlerSIGUSR1(int signal)
    {
        printf("C\n");
    }
    void handlerSIGCHLD(int signal)
    {
        int status;
        wait(&status);
        contor++;
        if(WIFEXITED(status))
            printf("Un fiu s-a incheiat cu codul:%d\n",WEXITSTATUS(status));
        else
        {
            perror("Un fiu nu s-a incheiat corect");
            exit(errorSignal--);
        }
    }
    int main(int argc,char *argv[])
    {
       if(argc!=3)
       {
        perror("Exemplu de apelare:prog_name file1 file2");
        exit(errorSignal--);
    }
    int pipeVal;
    pid_t c1,c2,c3;
    pipeVal=pipe(fdes);
    if(pipeVal<0)
    {
        perror("Nu se poate creea pipe-ul");
        exit(errorSignal--);
    }
    c1=fork();
    if(c1<0)
    {
        perror("Nu se poate creea primul fiu");
        exit(errorSignal--);
    }
    if(c1==0)
    {
        copil1(argv[1]);
        exit(0);
    }
    c2=fork();
    if(c2<0)
    {
        perror("Nu se poate creea al doilea fiu");
        exit(errorSignal--);
    }
    if(c2==0)
    {
        copil2(argv[2]);
        exit(0);
    }

    c3=fork();
    if(c3<0)
    {
        perror("Nu se poate creea al treilea fiu");
        exit(errorSignal--);
    }
    if(c3==0)
    {
        copil3();
    }
    close(fdes[0]);
    close(fdes[1]);
    struct sigaction sig;
    sig.sa_flags=0;
    sigemptyset(&(sig.sa_mask));
    sig.sa_handler=handlerSIGUSR1;
    if(sigaction(SIGUSR1,&sig,NULL)<0)
    {
        perror("SIGUSR1 error");
        exit(errorSignal--);
    }
    struct sigaction sig2;
    sig2.sa_flags=0;
    sigemptyset(&(sig2.sa_mask));
    sig2.sa_handler=handlerSIGCHLD;
    if(sigaction(SIGCHLD,&sig2,NULL)<0)
    {
        perror("SIGCHLD error");
        exit(errorSignal--);
    }

    while(contor!=3)
    {
        printf("1\n");
        sleep(1);
        fflush(stdout);
    }


    return 0;
}