#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/procfs.h>
#include <dirent.h>
#include "cshell.h"

//Helper to remove unwanted characters
void trunc_line(char * str)
{
    int len=0;
    char * cc = str;
    for(;*cc;cc++)if (*cc!=' ' && *cc!=':' && *cc!='\t')
            break;
    for(;*cc;cc++)str[len++]=*cc;
    str[len]='\0';
}

int print_details(char *filepath)
{
    //Parameters required 
    char *vmsize;
    char * state;
    char * pid;

    //Variable required
    char *line;
    size_t line_size,len;


    //Initializatons
    line_size = 1056;
    len = 1056;
    vmsize = NULL;
    state = NULL;
    pid = NULL;
    line = (char *)malloc(line_size*(sizeof(char)));


    //file pointer
    FILE * f = fopen(filepath, "r");
    if (!f) 
    {
        fprintf(stderr,"Error in opening file");
        return 1;
    }

    //Loop until parameters ae found
    while (!vmsize || !state || !pid )
    {
        if (getline(&line, &len, f) == -1)
        {
            //If we have reached the end of file break
            break;
        }
        //Use strdup to create duplicates and pass pointers to created
        if (!strncmp(line, "VmSize:", 7))
        {
            vmsize = strdup(&line[7]);
        }

        else if (!strncmp(line, "State:", 6))
        {
            state = strdup(&line[5]);
        }
        else if (!strncmp(line, "Pid:", 4))
        {
            pid = strdup(&line[5]);
        }

    }
    
    if (pid)
    {
        trunc_line(pid);
        printf("Pid : %s",pid);
    }
    if (state)
    {
        trunc_line(state);
        printf("State : %s",state);
    }
    if (vmsize)
    {
        trunc_line(vmsize);
        printf("Memory(virtual) : %s",vmsize);
    }
    //Mandatory free of memory and close of files
    fclose(f);

    free(line);
    free(state);
    free(vmsize);
    free(pid);

    //Success
    return 1;
}
/*
    Function - pinfo 
    Options - Empty or id
    Output - Returns info about process specified

*/
int pinfo_id(char ** args)
{
    //Initializations
    char * pid;
    char str[256],ex_path[256];
    int bufsize = 256;
    char buf[bufsize+1];


    if (args[1]==NULL)
        pid = shellid;
    else pid =args[1];
    
    //Prepare strings for querying
    strcpy(str,"/proc/");
    strcat(str,pid);
    strcpy(ex_path,str);
    strcat(str,"/status");
    strcat(ex_path,"/exe");

    print_details(str);

    //Last is the executable path
    int ret=readlink(ex_path, buf, bufsize);
    if (ret==-1)
        printf("Cannot read executable path!\n");
    else
    {
        buf[ret]='\0';
        printf("Executable Path: %s\n",buf);
    }
    return 1;
}
char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}
char * pinfostate(int  num)
{

    char pid[22];
    itoa(num,pid);
    printf("%s\n",pid);
    char str[256];
    //Prepare strings for querying
    strcpy(str,"/proc/");
    strcat(str,pid);
    strcat(str,"/status");
   
    //Parameters required 
    char * state;
    //Variable required
    char *line;
    size_t line_size,len;


    //Initializatons
    line_size = 512;
    len = 512;
    
    state = NULL;
    
    line = (char *)malloc(line_size*(sizeof(char)));


    //file pointer
    FILE * f = fopen(str, "r");
    if (!f) 
    {
        fprintf(stderr,"Error in opening file");
        return NULL;
    }

    //Loop until parameters ae found
    while ( !state )
    {
        if (getline(&line, &len, f) == -1)
        {
            //If we have reached the end of file break
            break;
        }

        else if (!strncmp(line, "State:", 6))
        {
            state = strdup(&line[5]);
        }

    }
    
   	fclose(f);

    free(line);
    if (state)
    {
        trunc_line(state);
        return state;
    }

    //Success
    return NULL;
}



