#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include "cshell.h"

int piping(char ** args)
{
    //Take 2 pipes
    int p0[2],p1[2];
    int i =0 , k = 0 , curind =0 ;
    char * cur;
    char * cmd[101];
    pid_t pid;   
    //Count the number of commands separated using pipes in the argument
    int num_args = 0 ;

    for(cur = args[0];cur;cur= args[++i])
        if (strncmp(cur,"|",1)==0)num_args++;
    
    //printf("The number of arguments are: %d\n",num_args)

    //start the code
    //run until NULL is found which sets end
    i=0;
    int end = 0;
    //printf("Reached piping function %d \n", num_args);

    while(!end)
    {
       // printf("Argument number %d \n",i);
        cur = args[curind];
        k = 0;
        //store the current command in cmd
        while(cur && strncmp(cur,"|",1)!=0)
        {     cmd[k++]=cur;
              cur = args[++curind];
        }
        curind++;
        cmd[k++]=NULL;
        //printf("Length of command is :%d \n", k);
        //if it is the last command the loop must be broken
        //in next iteration
        if (i == num_args)end=1;

        //Now we decide which pipe to use
        if (i%2==0)pipe(p0);
        else pipe(p1);

        //Create child process
        pid = fork();
        if (pid < 0)
            perror("Error in creating pipe");
        
        else if (pid == 0)
        {
            //Functionality for redirection also
            char ** temp = redirect(cmd);
            
            //Case 1: First command: only ouput settle required
            if (i==0)dup2(p0[1],1);
            else if (end)
            {
                // Case 2:Last element: only input settle required
                // if i is odd read from even vice-versa
                if (i%2 == 0)
                    dup2(p1[0],0);
                else dup2(p0[0],0);
            }
            else
            {
                //Middle elements
                if (i%2 == 0)
                {
                    //Read from odd into even
                    dup2(p1[0],0);
                    dup2(p0[1],1);
                }
                else
                {
                    //Read from even into odd
                    dup2(p0[0],0);
                    dup2(p1[1],1);
                }
            }
            if (execvp(temp[0],temp)== -1)
                perror("Error in executing command");
        }
        else
        {
            //Case 1: Start element
            if (i==0)close(p0[1]);
            else if (end)
            {
                //Case 2: Last element
                if (i%2)close(p0[0]);
                else close(p1[0]);
            }
            else
            {
                // Case 3: Middle elements
                if (i%2){
                    close(p0[0]);close(p1[1]);
                }
                else {close(p1[0]);close(p0[1]);}
            }

            //Wait for child 
            waitpid(pid,NULL,0);
             //Move 1 step further to access the arg after pipe
            i++;
        }

    }
    return 1;
}



















    




