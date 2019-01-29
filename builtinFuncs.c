#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "cshell.h"



int cd(char **tokens)
{
	char* destination = (char*)malloc(200*sizeof(char));
	int i, len;
    if (tokens[1]==NULL){           
        chdir(myhome);
        return 1;
    }
	if(tokens[1][0] == '~')
	{
		strcpy(destination,home);
		len = strlen(destination);

		for(i=1;tokens[1][i]!='\0';i++)
		{
			destination[len++]=tokens[1][i];
		}
	}
	else
		strcpy(destination, tokens[1]);

	int test = chdir(destination);
	if(test == -1)
		perror("Error ");
    return 1;
}

int pwd()
{
	char *dir = (char*)malloc(1000*sizeof(char));
	dir = getcwd(dir,1000);
	if(dir != NULL)
	{
		write(1,dir,strlen(dir));
		write(1,"\n",1);
	}
	else
		perror("Error ");
    return 1;
}

int echo(char **arg)
{
	
	int i=0,j=0;
    for(j=1;arg[j]!=NULL;j++)
    {
        i=0;
        while(1)
        {
            if(arg[j][i]=='\0')
            {
                break;
            }
	    else if(arg[j][i] == 34)
	    { 
		i++;
		continue;
	    }
            else
	    {
                fprintf(stdout,"%c",arg[j][i]);
                i++;
            }
        }
        printf(" ");
    }
    printf("\n");
    return 1;
}


int exit_shell(char ** args)
{
    printf("Hope to see you soon!\n");
    exit(EXIT_SUCCESS);
    return 0;
}

int setVar(char **args){
    int i= 0;
    int argc = i;
    while (args[argc]!=NULL)
        argc++;

    int flag = (argc == 2 || argc == 3);
    if (flag){
        int f1 = strcmp(args[0], "setenv");
        if (f1){
            char  c;
            int f11 = (argc == 2);
            if (f11) 
                if (setenv(args[1], &c, 1) != 0)
                    printf("Error in executing command\n"); 
            int f12 = (argc == 3);
            if (f12){
                int f111 = (setenv(args[1], args[2], 1) != 0);
                if (f111)
                    printf("Error in executing command\n");
            }
        }
    
    }
    else {
        printf("Error in usage\n");
        return 1;
    }
}

int unsetVar(char **args) {
    int i=0;
    int argc = i;
    while(args[argc])
        argc = argc+1;

    int f1 = (argc == 2);
    if (f1) {
        int f2 = (unsetenv(args[1])!=0);
        if (f2)
            printf("Error in executing command\n");
    }
    else {
        printf("Error in usage\n");
    }
}

int jobs(char **args){
    int iter=1;
    int cnt =0;
    while(iter < sigsize+1){

        int f1 = (proc[iter] && proc[iter]->state);
        if(f1){
            cnt = cnt+1;
            printf("[%d] ",  cnt );
            printf ("%s" , pinfostate(proc[iter]->num));
            printf(" %s ", (proc[iter])->pname );
            printf("[%d]\n", (proc[iter])->num );
            proc[cnt]= proc[iter];
        }
        iter = iter+1;
    }

    sigsize = cnt;
    return 1;
}

int kjob(char **args){

    int f1 = (args[1]==NULL || args[2]==NULL);
    if (f1)
        perror("Error in Usage");
    
    int iter =1 ;
    int cnt =0;
    int signum = atoi(args[2]);
    int procnum =atoi(args[1]);
    while(iter < sigsize+1)
    {
        int f2 = (proc[iter] && proc[iter]->state);
        if (f2){
            cnt = cnt+1;
        }
        int f3 = (cnt == procnum);
        if(f3)
            break;
        iter +=1;
    }
  

    //Fix segmentation fault
    int f4 = procnum>sigsize || proc[iter]==NULL || proc[iter]->state==0 ;
    if(f4){
        printf("The process does not exist.\n" );
        return 1;
    }
    int id = proc[iter]->num;
    int f5 = (id == 9 || id== 2 || id == 15) ;
    if (f5 )
    {
        proc[iter]=NULL;
        proc[iter]->state = 0;
    }

    kill(id, signum); 

    return 1;
}

int fg(char **args){

    int f1 = (args[1]==NULL);
    if(f1){
        printf("Usage: fg [process no.]\n");
    }
    int i =0;

    int procnum = atoi(args[1]);
    int iter = 1 ,  cnt = 0;
    while(iter <= sigsize)
    {
    int f2  = (proc[iter] && proc[iter]->state);
    if (f2){
        cnt++;
    }
    int f3  = (cnt == procnum);
    if (f3)break;
    iter +=1 ;
    }
    int f4 = (procnum>sigsize || proc[iter]==NULL || proc[iter]->state==0);
    if(f4){
    perror("No such process exists.\n" );
    return 1;
    }

    
    int id = proc[iter]->num;
    proc[iter]= NULL;

    int status  = 0, lol = 1;
    pid_t wpid;
    int f5 = (kill(id ,18 )!=0);
    if (f5)
        lol =1 ;
    wpid = waitpid(id, &status, WUNTRACED);
    //while (!WIFEXITED(status) && !WIFSIGNALED(status));
    
    return 1;
}

int bg(char **args){

    int f1 = (args[1]==NULL);
    if(f1){
        printf("Usage: bg [process no.]\n");
    }
    int i=0;
    int procnum = atoi(args[1]);
    int iter = 1 ,  cnt = 0;
    while(iter <  sigsize+1)
    {

        int f2 = (proc[iter] && proc[iter]->state);
        if (f2)cnt++;

        int f3 = (cnt == procnum);
        if (f3){
            break;
        }
        iter +=1 ;
    }

    int f4 = (procnum>sigsize || proc[iter]==NULL || proc[iter]->state==0);
    if(f4){
    perror("No such  process exists.\n" );
    return 1;
    }

    //kill(id, 18);
    int id = proc[iter]->num;
    kill(id, 18);
    return 1;
}
int overkill(char **args)
{

    int iter = 1;
    for(iter =1;iter <= sigsize ;iter++)
    {
        if(proc[iter])
        {
            int f2  = (kill(proc[iter]->num,9)!=0);
            if (f2)
                perror("Error in killing process");
            proc[iter]=NULL;
        }
    }
    sigsize= 0;

    return 1;

}
