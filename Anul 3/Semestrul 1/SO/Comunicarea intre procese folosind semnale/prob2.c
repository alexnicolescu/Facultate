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


pid_t child_pid;
int n;

void process_a_ends()
{
    int status;
    wait(&status);
    printf("Fiul s-a incheiat cu %d\n",WIFEXITED(status));
    printf("Procesul parinte s-a incheiat\n");
    exit(0);
}

void alarmHandler(int signal)
{
    if(kill(child_pid,SIGUSR1)<0)
    {
        printf("Eroare la trimiterea semnalului SIGUSR1 copilului\n");
        exit(1);
    }
    return;
}
void process_a()
{   
    struct sigaction process_a_action,process_a_action2;
    process_a_action.sa_flags=0;
    sigemptyset(&(process_a_action.sa_mask));
    process_a_action.sa_handler=process_a_ends;

    if(sigaction(SIGCHLD,&process_a_action,NULL)<0)
    {
        perror("sigaction SIGTERM ignore");
        exit(-3);
    }
    alarm(7);
    process_a_action2.sa_flags=0;
    sigemptyset(&(process_a_action2.sa_mask));
    process_a_action2.sa_handler=alarmHandler;
    if(sigaction(SIGALRM,&process_a_action2,NULL)<0)
    {
        perror("sigaction SIGALRM ignore");
        exit(-4);
    }
    while(1)
    {
        printf("A\n");
    }
    exit(0);
}

void process_b_ends()
{
    printf("Procesul b se incheie\n");
    exit(0);
}

void process_b()
{

    struct sigaction process_b_action;
    process_b_action.sa_flags=0;
    sigemptyset(&(process_b_action.sa_mask));
    process_b_action.sa_handler=process_b_ends;

    if(sigaction(SIGUSR1,&process_b_action,NULL)<0)
    {
        perror("sigaction SIGUSR1 ignore");
        exit(-3);
    }

    while(1)
    {
        printf("B\n");
    }
    exit(0);
}

int main()
{   

   struct sigaction action_ignore;
   action_ignore.sa_flags=0;
   sigemptyset(&(action_ignore.sa_mask));
   action_ignore.sa_handler=SIG_IGN;
   if(sigaction(SIGTERM,&action_ignore,NULL)<0)
   {
    perror("sigaction SIGTERM ignore");
    exit(-2);
}
if((child_pid=fork())<0)
{
    perror("Eroare la crearea procesului fiu");
    exit(1);
}
if(child_pid==0)
{
    process_b();
    exit(0);

}
else
{
    process_a();
}




return 0;
}