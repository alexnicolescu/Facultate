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

int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        perror("Trebuie 2 argumente.Exemplu de apel:nume_prog nr1 nr2");
        exit(1);
    }
    int pfd1[2];
    int pfd2[2];
    pid_t pidp,pidc;
    if(pipe(pfd1)<0)
    {
        perror("Nu se poate crea primul pipe");
        exit(-1);
    }

    int numarProducatori,numarConsumatori;
    numarProducatori=atoi(argv[1]);
    numarConsumatori=atoi(argv[2]);
    for(int i=0;i<numarProducatori;i++)
    {
        pidp=fork();
        if(pidp==0)
        {
           close(pfd1[0]);
            dup2(pfd1[1],1);
            execlp("/home/alexnicolescu/fis1","fis1",NULL);
            printf("Eroare la exec\n");
            exit(1);
        }
    }
    pidp=fork();
    if(pidp==0)
    {
      close(pfd1[0]);
      dup2(pfd1[1],1);
      execlp("ls","ls","-l",NULL);
      printf("Eroare la exec\n");
      exit(1);
  }
  
  char ch='*';
  srand(time(NULL));
  int r=rand()%20;
  for(int i=0;i<r;i++)
    {
       if((write(pfd1[1],(void *)(&ch),sizeof(char)))==-1)
       {
        perror("Eroare");
        exit(-12);
       }

    }

if(pipe(pfd2)<0)
{
    perror("Eroare la crearea celui de-al doilea pipe");
    exit(-1);
}

for(int i=0;i<numarConsumatori;i++)
{
    pidc=fork();
    if(pidc<0)
    {
        perror("Eroare la fork");
        exit(-9);
    }
    if(pidc==0)
    {
        char ca;
        int nrOcteti=0;
        FILE *stream;
        close(pfd1[1]);

        close(pfd2[0]);
        
        while((read(pfd1[0],(void *)(&ca),sizeof(char)))>0)
        {
            printf("PID:%d,Caracter:%c\n",getpid(),ca);
            nrOcteti++;

        }
        close(pfd1[0]);
        
        stream=fdopen(pfd2[1],"w");
        int nr=i+1;
        fprintf(stream,"%d:%d\n",nr,nrOcteti);
        fclose(stream);


        exit(42);
    }
}
close(pfd1[0]);
close(pfd1[1]);
int status;

close(pfd2[1]);

char info[20];
int n;
int dim=2*sizeof(int) + sizeof(':') + sizeof('\n');
while((n=read(pfd2[0],(void *)info,dim))>0)
{
    info[n]='\0';
    printf("%s\n",info);

}

close(pfd2[0]);
for(int j=0;j<numarProducatori;j++)
{
    wait(&status);
    if(!WIFEXITED(status))
    {
        printf("Unul din fi nu s-a terminat corect:%d\n",WEXITSTATUS(status));
        exit(-4);
    }
}
for(int j=0;j<numarConsumatori;j++)
{
    wait(&status);
    if(!WIFEXITED(status))
    {
        printf("Unul din fi nu s-a terminat corect:%d\n",WEXITSTATUS(status));
        exit(-4);
    }
}

return 0;
}