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


//pid_t child_pid;
int n;
void process_a_ends()
{
  printf("Programul s-a incheiat\n");
  exit(0);
}
void incrementNumber()
{
  n++;
}
void alarmHandler()
{
  printf("Alarm s-a activat\n");
  return;
}
void setAlarm()
{
  alarm(5);
  struct  sigaction alarm;
  alarm.sa_flags=0;
  sigemptyset(&(alarm.sa_mask));
  alarm.sa_handler=alarmHandler;
  if(sigaction(SIGALRM,&alarm,NULL)<0)
  {
    perror("Process a SIGALARM sigaction");
    exit(-3);
  }
  sleep(5);
  n++;



}
void process_a()
{   struct  sigaction process_a_sigterm_action;
  process_a_sigterm_action.sa_flags=0;
  sigemptyset(&(process_a_sigterm_action.sa_mask));
  process_a_sigterm_action.sa_handler=process_a_ends;
  if(sigaction(SIGTERM,&process_a_sigterm_action,NULL)<0)
  {
    perror("Process a SIGTERM sigaction");
    exit(-1);
  }
  process_a_sigterm_action.sa_handler=incrementNumber;
  if(sigaction(SIGUSR1,&process_a_sigterm_action,NULL)<0)
  {
    perror("Process a SIGUSR1 sigaction");
    exit(-2);
  }
  process_a_sigterm_action.sa_handler=setAlarm;
  if(sigaction(SIGUSR2,&process_a_sigterm_action,NULL)<0)
  {
    perror("Process a SIGUSR2 sigaction");
    exit(-2);
  }
  while(1){

    printf("%d %d\n",n,n);
    sleep(1);

  }
}

int main()
{   
  n=0;
  process_a();
  return 0;
}