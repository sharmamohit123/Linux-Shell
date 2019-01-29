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
#include <grp.h>
#include <time.h>
#include <signal.h>
#include <sys/utsname.h>


#include "cshell.h"


int launch(char **args)
{
    
    pid_t pid;
    pid_t wpid;
    pid_t ind=0;

    int status;
    int back_ground=0;
    int i;
    
    for(i=1;args[i];i+=1)
        ind=i;

    int f1  = (ind && strcmp(args[ind],"&")==0);
    if (f1){
        back_ground=1;
    }

    pid=fork();
    // parent process in background adding it to the proc array
    int f2  = (pid > 0 && back_ground == 1);
    if (f2){
        signal(SIGCHLD, sigh);
        sigsize=sigsize+1;
        proc[sigsize] = (process *)(malloc(sizeof(process)));
        proc[sigsize]->num = pid;
        strcpy((proc[sigsize])->pname, args[0]);
        proc[sigsize]->state = 1;
        printf("\n");

        return 1;
    } 
 
    //Error handler
    int f3 = ((pid)<0);
    int f4 = (pid==0);
    if (f3)
    {

        fprintf(stderr, "Error cannot initiate process!\n");
    }
    else if (f4){
        args = redirect(args);
        //Child process

        int f5 = (execvp(args[0],args) == -1);
        if(f5)
        {
          fprintf(stderr, "Error in executing command\n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);


    }

    else {
        // Parent process
        // If process is foreground then wait else return
        int f6 = (!back_ground);
        if (f6)
        {
             foreground_id = pid;
            curname = args[0];
            signal(SIGTSTP, sigz);
            
            wpid = waitpid(pid, &status, WUNTRACED);
        }
    }
    return 1;
}
