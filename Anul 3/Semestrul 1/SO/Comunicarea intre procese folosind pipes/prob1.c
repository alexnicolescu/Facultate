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


int main()
{
    int pfd1[2],pfd2[2],pfd3[2];
    int fd,n=0,status1,val;
    char buffc[4096];
    pid_t pid,pid2;
    if(pipe(pfd1)<0)
    {
        perror("Eroare la crearea primului pipe");
        exit(-1);
    }
    if(pipe(pfd2)<0)
    {
        perror("Eroare la crearea celui de-al doilea pipe");
        exit(-1);
    }
    if(pipe(pfd3)<0)
    {
        perror("Eroare la crearea celui de-al doilea pipe");
        exit(-1);
    }
    pid=fork();
    if(pid<0)
    {
        perror("Eroare la crea primului fiu");
        exit(-2);
    }
    if(pid==0)
    {  

        char c[4096];
        char buff[4096];
        int nr=0,n;
        close(pfd1[1]);
        close(pfd2[0]);
        close(pfd3[1]);
        close(pfd3[0]);
        while((n=read(pfd1[0],(void *)c,4096))>0)
        {   

            for(int i=0;i<n;i++)
                if(isalpha(c[i])&&islower(c[i]))
                {
                  buff[nr++]=c[i];
              }
          }
          if(nr!=0){
              if(write(pfd2[1],(void *)buff,nr)==-1)
              {
               perror("Nu se pot adauga date in pipe");
               exit(-7);       
           }
       }
       close(pfd1[0]);
       close(pfd2[1]);
       exit(1);


   }
   close(pfd1[0]);
   fd=open("date.txt",O_RDONLY);
   if(fd==-1)
   {
    perror("Fisierul date.txt nu poate fi deschis");
    exit(-5);
}
while((n=read(fd,(void *)buffc,4096))>0)
{
    if(write(pfd1[1],(void *)buffc,n)==-1)
    {
        perror("Nu se pot adauga date in pipe");
        exit(-7);
    }
}
if(n==-1)
{
    perror("Nu se poate citi din fisierul date.txt");
    exit(-6);
}
close(pfd1[1]);

if(close(fd)==-1)
{
    perror("Nu se poate inchide fisierul data.txt");
    exit(-8);
}
pid2=fork();
if(pid2<0)
{
    perror("Eroare la crearea celui de-al doilea fiu");
    exit(-2);
}
if(pid2==0)
{
    int fd2;
    int tab[27]={0};
    int nr=0,n;
    char c[4097],linie[9];
    close(pfd2[1]);
    close(pfd3[0]);
    fd2=open("statistica.txt",O_WRONLY|O_CREAT|O_TRUNC,0777);
    if(fd2==-1)
    {
        perror("Eroare la crearea fisierului statistica.txt");
        exit(-4);
    }
    while((n=read(pfd2[0],(void *)c,4096))>0)
    {
        for(int i=0;i<n;i++)
        tab[c[i]-'a']++;
    }
    close(pfd2[0]);
    for(int i=0;i<27;i++)
    {
        if(tab[i]!=0)
        {
            nr++;
            snprintf(linie,9,"%c:%d\n",'a'+i,tab[i]);
            if(write(fd2,(void *)linie,strlen(linie))==-1)
            {
                perror("Nu se poate scrie in fisierul statistica.txt");
                exit(-14);
            }
        }
    }
    if(write(pfd3[1],(void *)(&nr),sizeof(int))==-1)
    {
        perror("Nu se pot adauga date in pipe");
        exit(-4);
    }
    close(pfd3[1]);
    close(fd2);
    exit(2);

}
close(pfd2[1]);
close(pfd2[0]);
close(pfd3[1]);
if((read(pfd3[0],(void *)(&val),sizeof(int)))>0)
{
    printf("Numarul de litere mici distincte:%d\n",val);
}
else{
    perror("Eroare la citirea din pipe");
    exit(-14);
}
close(pfd3[0]);
for(int i=0;i<2;i++){
    wait(&status1);
    if(WIFEXITED(status1))
    {
        printf("Fiul %d s-a terminat corect\n",i+1);
            //exit(-12);
    }
}



return 0;
}