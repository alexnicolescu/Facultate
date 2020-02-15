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
//un program cu p + 3c
//p va printa in continuu mesajul parinte pana cand isi termina executia ultimul copil
//copilul unu va rula gcc,pentru un fisier primit ca si argument in linie de comanda
//copilul 2 va printa in continuu mesajul copil,pana cand primeste semnalul SIGUSR1
//copilul 3 va procesa informatia(numara caractere) produsa de gcc,rulat de copilul 1,folosindu-se de un pipe.Cand termina de numarat va trimite SIGUSR1 la copilul 2
//

int fdes[2];
int errorSignal=-1;
int contor=0;

void copil1(char *cale)
{
	close(fdes[0]);
	
	int dupVal;
	dupVal=dup2(fdes[1],2);
	if(dupVal<0)
	{
		perror("Eroare la functia dup2");
		exit(errorSignal--);
	}
	execlp("gcc","gcc","Wall",cale,NULL);
	close(fdes[1]);
	perror("Eroare la exec");
	exit(errorSignal--);
}
void handlerUSR1(int signal)
{
	exit(1);
}
void copil2(void)
{
	close(fdes[0]);
	close(fdes[1]);
	struct sigaction sig;
	sig.sa_flags=0;
	sigemptyset(&(sig.sa_mask));
	sig.sa_handler=handlerUSR1;
   	if(sigaction(SIGUSR1,&sig,NULL)<0)
   	{
   		perror("SIGUSR1 error");
   		exit(errorSignal--);
   	}
   	while(1)
   		printf("Copil\n");

}
void copil3(pid_t c2)
{
	close(fdes[1]);
	char c;
	int n,nr=0;
	while((n=read(fdes[0],(void *)(&c),1))>0)
	{
		nr++;
	}
	if(n==-1)
	{
		perror("Nu se poate citi din pipe");
		exit(errorSignal--);
	}
	close(fdes[0]);
	if(kill(c2,SIGUSR1)<0)
	{
		perror("Nu se poate trimite semnalul SIGUSR1");
		exit(errorSignal--);
	}
	exit(1);
}

void handlerSIGCHLD(int signal)
{
	int status;
	wait(&status);
	contor++;
	if(!WIFEXITED(status))
	{
		perror("Unul dintre copii nu s-a incheiat cu bine");
		exit(errorSignal--);
	}
}
int main(int argc,char *argv[])
{

	if(argc!=2)
	{
		perror("Numar incorect de argumente.Exemplu de apelare ./prog fis.c");
		exit(errorSignal);
	}

	int pipeVal;
	pid_t c1,c2,c3;
	pipeVal=pipe(fdes);
	if(pipeVal<0)
	{
		perror("Nu se poate crea un pipe");
		exit(errorSignal--);
	}
	c1=fork();
	if(c1<0)
	{
		perror("Nu se poate crea primul copil");
		exit(errorSignal--);
	}
	if(c1==0)
	{
		copil1(argv[1]);
	}
	c2=fork();
	if(c2<0)
	{
		perror("Nu se poate crea al doilea copil");
		exit(errorSignal--);
	}
	if(c2==0)
	{
		copil2();
	}
	c3=fork();
	if(c3<0)
	{
		perror("Nu se poate crea al treilea copil");
		exit(errorSignal--);
	}
	if(c3==0)
	{
		copil3(c2);
	}
	close(fdes[0]);
	close(fdes[1]);
	struct sigaction sig;
	sig.sa_flags=0;
	sigemptyset(&(sig.sa_mask));
	sig.sa_handler=handlerSIGCHLD;
   	if(sigaction(SIGCHLD,&sig,NULL)<0)
   	{
   		perror("SIGCHLD error");
   		exit(errorSignal--);
   	}
   	while(contor!=3)
   		printf("Parinte\n");

return 0;
}
