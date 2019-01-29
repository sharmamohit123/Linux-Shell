#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/utsname.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include "cshell.h"



char **redirect (char ** args){

int fdin, fdout, fdappend;
char inp[100],out[100],app[100];

 bool isinp,isout,isappend;
 isinp = isout = isappend=0;

char * curarg;
int curind =0;

for(curarg = args[curind];curarg ; curarg=args[++curind])
{
  int f1 = (strncmp(curarg,"<",1)==0);
  int f2 = (strncmp(curarg,">",1)==0);
  int f3 = (strncmp(curarg,">>",2)==0);
  if (f1){
    args[curind]=NULL;
    isinp = true;
    strcpy(inp,args[curind+1]);
  }

  else if(f2){
    args[curind]=NULL;
    isout= true;
    strcpy(out,args[curind+1]);
  }

  else if(f3){
    args[curind]=NULL;
    isappend = true;
    strcpy(app,args[curind+1]);
  }

}

int f4 = (!( isinp || isout || isappend));
if(f4)
  return args;

int  f5 = isinp;
if(f5){

  fdin = open(inp, O_RDONLY,0);

  int f6 = (fdin < 0);
  if(f6){
    perror("Error in opening input file");
    exit(0);
  }

  dup2(fdin,0);
  close(fdin);
}

int f7 = isout;
if(f7){
  fdout = open (out, O_WRONLY | O_CREAT, 0666);
  
  int f8 = (fdout<0);
  if(f8){

    perror("Error in opening output file");
    exit(0);
  }

  dup2(fdout,1);
close(fdout);
}

int f9 = (isappend==1);
if(f9){
  fdappend = open (app, O_WRONLY | O_CREAT | O_APPEND , 0666);
  int f10 = (fdappend<0);
  if(f10){
    perror("Error in opening output file");
    exit(0);
  }

  dup2(fdappend,1);
  close(fdappend);
}

return args;
}
