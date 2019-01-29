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
#include <cshell.h>


char *built_cmnds[] = {"overkill", "bg", "fg", "kjob", "jobs", "unsetenv", "setenv", "quit", "pinfo", "ls", "cd", "echo", "pwd"};

int exec_builtin(int id,char ** args)
{
    switch(id){
        case 0:
            return overkill(args);
            break;
        case 1:
            return bg(args);
            break;
        case 2:
            return fg(args);
            break;
        case 3:
            return kjob(args);
            break;
        case 4:
            return jobs(args);
            break;
        case 5:
            return unsetVar(args);
            break;
        case 6:
            return setVar(args);
            break;
        case 7:
            return exit_shell(args);
            break;
        case 8:
            return pinfo_id(args);
            break;
        case 9:
            return ls(args);
            break;
        case 10:
            return cd(args);
            break;
        case 11:
            return echo(args);
            break;
        case 12:
            return pwd();
            break;
        default:
            return 0;
    }
}


int execute(char ** args)
{    
    //Check if any command was entered
    if (args[0] == NULL){

        return 1;
    }
    int i =0;
    char * cur;
    cur=args[i];
    while (cur){
        if (!strncmp(cur,"|",1)){
            return piping(args);
        }
        i=i+1;
        cur=args[i];
    }

    //Check if it was a built in command
    int n =sizeof(built_cmnds) / sizeof(char *);
    i=0;
    while(i<=n-1){
        if(!strcmp(args[0], built_cmnds[i])){
            return exec_builtin(i,args);
        }
        i=i+1;
    }

    return launch(args);

}
