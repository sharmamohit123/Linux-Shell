#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cshell.h"


void print_prompt()
{
    printf("%s@%s:",myname->pw_name,hostname);
    char cwd[1024];
    int f1 = ((getcwd(cwd, sizeof(cwd)) != NULL));
    if (f1)
    {
        int ret;
        ret = strcmp(cwd,myhome);
        int f2 = (ret==0);
        int f3 = (ret<0);
        if (f2)
            printf("~> ");
        else if (f3)
            printf("%s> ",cwd);
        else{
            int len;
            len = strlen(myhome);
            printf("~%s> ",cwd+len);
        }
    }
    else
        perror("error in accessing directory");
}

