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

/*
    Function - ls
    Input - Empty or directory
    Options - -l,-a
    Output - List of files with info

*/

void list_dir(char *dire, char *flag){

    char cwd[1024];
    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    char name[1000];
    dir = opendir(dire);
    int f1 = (dir == 0) ;
    if (f1) {
        perror ("NO such directory exists");
        return;
    }
    while ((dirEntry=readdir(dir))!=0) {
        
        int f2 = (flag == NULL);
        if(f2){
            int f2 = ((dirEntry->d_name)[0]=='.');
            if (f2){
                continue;
            }
        }

        printf("%s ", dirEntry->d_name);
    }
    printf("\n");
    return;
}

void detailed_list_dir(char *dire, char *flag)
{
    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    char name[1000], cwd[1024];
    dir = opendir(dire);
    int f1 = (dir == 0);
    if (f1) {
        perror ("NO such directory exists");
        return;
    }

    while ((dirEntry=readdir(dir)) != 0) {
        // For checking whether directory or file
        int f2  = (strcmp(flag, "-l")==0);
        if(f2){
            int f3 = ((dirEntry->d_name)[0]=='.');
            if (f3)
                continue;
        }
        stat(dirEntry->d_name, &inode);
        int f4 = S_ISDIR(inode.st_mode); 
        if(f4) 
            printf("d");
        else 
            printf("-");     

        // Permissions for user
        int f5 = inode.st_mode & S_IRUSR;
        if (f5) 
            printf("r");
        else 
            printf("-");

        int f6 = inode.st_mode & S_IWUSR; 
        if (f6) 
            printf("w");
        else 
            printf("-");

        int f7 = inode.st_mode & S_IXUSR; 
        if (f7) 
            printf("x");
        else 
            printf("-");

        // Permissions for groups

        int f8 = inode.st_mode & S_IRGRP; 
        if (f8) 
            printf("r");
        else 
            printf("-");

        int f9 = inode.st_mode & S_IWGRP; 
        if (f9) 
            printf("w");
        else 
            printf("-");

        int f10 = inode.st_mode & S_IXGRP; 
        if (f10) 
            printf("x");
        else 
            printf("-");

        // Permissions for others

        int f11 = inode.st_mode & S_IROTH; 
        if (f11) 
            printf("r");
        else 
            printf("-");

        int f12 = inode.st_mode & S_IWOTH;
        if (f12) 
            printf("w");
        else 
            printf("-");

        int f13 = inode.st_mode & S_IXOTH; 
        if (f13) 
            printf("x");
        else 
            printf("-");

        printf(" ");

        // Print number of hard-links
        printf("%d ", (int)inode.st_nlink);

        // Print Owner name
        struct passwd *pw = getpwuid(inode.st_uid);
        int f14 = (pw!=0);
        if (f14) 
            printf("%s ", pw->pw_name);

        // Print Group name
        struct group  *gr = getgrgid(inode.st_gid);
        int f15 = (gr!=0);
        if (f15) 
            printf("%s ", gr->gr_name);

        // Print number of size in bytes
        printf("%d ",(int)inode.st_size);

        // Print time of modification
        time_t t = inode.st_mtime;
        struct tm lt;
        localtime_r(&t, &lt);
        char timbuf[80];
        strftime(timbuf, sizeof(timbuf), "%b %d %H:%M ", &lt);
        printf("%s", timbuf);
        printf("%s\n", dirEntry->d_name);
        // 
    }
    return;
}

int ls(char **argv){

    int f1 = (argv[1] == NULL || ((strcmp(argv[1],"-a")==0) && argv[2] == NULL));
    if (f1) {
        char tmp[10];
        tmp[0] = '.'; tmp[1] = '\0';
        list_dir(tmp, argv[1]);
        return 1;
    }
    int f2 = (((strcmp(argv[1], "-l")==0) && (argv[2] == NULL)) || ((strcmp(argv[1], "-la")==0) && (argv[2] == NULL))
     || ((strcmp(argv[1], "-al")==0) && (argv[2] == NULL)));
    if (f2){
        char tmp[10];
        tmp[0] = '.'; tmp[1] = '\0';
        detailed_list_dir(tmp, argv[1]);
        return 1;
    }
    int f3 = ((argv[1][0] != '-' && argv[2] == NULL)|| ((strcmp(argv[1],"-a")==0) && argv[2][0] != '-' && argv[3] == NULL) || 
        (argv[1][0]!='-' && (strcmp(argv[2],"-a")==0)  && argv[3] == NULL));
    if(f3)
    {   

        int f4 = (argv[1][0] != '-');
        if (f4)
            list_dir(argv[1], argv[2]);
        else 
            list_dir(argv[2], argv[1]);
        return 1;
    }
    int f5 = (argv[1][0] != '-');
    if (f5){
        int f7 = (((strcmp(argv[2], "-l")==0) && (argv[3] == NULL)) || ((strcmp(argv[2], "-la")==0) && (argv[3] == NULL))
                    || ((strcmp(argv[2], "-al")==0) && (argv[3] == NULL)));
        if (f7)
            detailed_list_dir(argv[1], argv[2]);
        else  
            printf("No such command exists\n");
        return 1;
    }

    int f6 = (argv[2]!=NULL && argv[2][0] != '-');
    if(f6){
        int f8 = (((strcmp(argv[1], "-l")==0) && (argv[3] == NULL)) || ((strcmp(argv[1], "-la")==0) && (argv[3] == NULL))
                    || ((strcmp(argv[1], "-al")==0) && (argv[3] == NULL)));
        if (f8)
            detailed_list_dir(argv[2], argv[1]);
        else  
            printf("No such command exists\n");
        return 1;
    }
    
    printf("No such command exists\n");
    return 1;
}
