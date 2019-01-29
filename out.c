10,12c10,44
< #include "cshell.h"
< 
< int sigsize = 0;
---
> #include <dirent.h>
> #include <pwd.h>
> #include <grp.h>
> #include <time.h>
> 
> /*
>     Function - ls
>     Input - Empty or directory
>     Options - -l,-a
>     Output - List of files with info
> 
> */
> 
> void list_dir(char *dire, char *flag){
> 
>     char cwd[1024];
>     DIR* dir;
>     struct dirent *dirEntry;
>     struct stat inode;
>     char name[1000];
>     dir = opendir(dire);
>     if (dir == 0) {
>         perror ("NO such directory exists");
>         return;
>     }
>     // printf("total %d\n", );
>     while ((dirEntry=readdir(dir)) != 0) {
>         if(flag == NULL)
>             if ((dirEntry->d_name)[0]=='.')
>                 continue;
>         printf("%s ", dirEntry->d_name);
>     }
>     printf("\n");
>     return;
> }
14c46
< void setshellid(int cur_id)
---
> void detailed_list_dir(char *dire, char *flag)
16,22c48,124
< 	char temp[10];
< 	int j = 0;
< 	for (int i=cur_id;i>0;i=i/10) temp[j++] = (i%10) + '0';
< 	temp[j] = '\0';
< 	int k = 0;
< 	for (int i=j-1;i>=0;i--) shellid[k++] = temp[i];
< 	shellid[k] = '\0';
---
>     DIR* dir;
>     struct dirent *dirEntry;
>     struct stat inode;
>     char name[1000];
>     char cwd[1024];
>     dir = opendir(dire);
>     if (dir == 0) {
>         perror ("NO such directory exists");
>         return;
>     }
>     // printf("total %d\n", );
>     while ((dirEntry=readdir(dir)) != 0) {
>         // For checking whether directory or file
>         if(strcmp(flag, "-l")==0)
>             if ((dirEntry->d_name)[0]=='.')
>                 continue;
>         stat(dirEntry->d_name, &inode);
>         if(S_ISDIR(inode.st_mode)) printf("d");
>         else printf("-");     
> 
>         // Permissions for user
>         if (inode.st_mode & S_IRUSR) printf("r");
>         else printf("-");
> 
>         if (inode.st_mode & S_IWUSR) printf("w");
>         else printf("-");
> 
>         if (inode.st_mode & S_IXUSR) printf("x");
>         else printf("-");
> 
>         // Permissions for groups
>         if (inode.st_mode & S_IRGRP) printf("r");
>         else printf("-");
> 
>         if (inode.st_mode & S_IWGRP) printf("w");
>         else printf("-");
> 
>         if (inode.st_mode & S_IXGRP) printf("x");
>         else printf("-");
> 
>         // Permissions for others
>         if (inode.st_mode & S_IROTH) printf("r");
>         else printf("-");
> 
>         if (inode.st_mode & S_IWOTH) printf("w");
>         else printf("-");
> 
>         if (inode.st_mode & S_IXOTH) printf("x");
>         else printf("-");
> 
>         printf(" ");
> 
>         // Print number of hard-links
>         printf("%d ", (int)inode.st_nlink);
> 
>         // Print Owner name
>         struct passwd *pw = getpwuid(inode.st_uid);
>         if (pw!=0) printf("%s ", pw->pw_name);
> 
>         // Print Group name
>         struct group  *gr = getgrgid(inode.st_gid);
>         if (gr!=0) printf("%s ", gr->gr_name);
> 
>         // Print number of size in bytes
>         printf("%d ",(int)inode.st_size);
> 
>         // Print time of modification
>         time_t t = inode.st_mtime;
>         struct tm lt;
>         localtime_r(&t, &lt);
>         char timbuf[80];
>         strftime(timbuf, sizeof(timbuf), "%b %d %H:%M ", &lt);
>         printf("%s", timbuf);
>         printf("%s\n", dirEntry->d_name);
>         // 
>     }
>     return;
25,44c127,143
< int main(int argc, char const *argv[])
< {
< 	
< 	int cur_id = getpid();
< 	setshellid(cur_id);
< 
<     char * myline;
<     char ** arg_array;
<     char ** args;
<     getcwd(myhome, sizeof(myhome));
<     myname = getpwuid(getuid());
<     gethostname(hostname, 29);
< 
<     print_prompt();
< 
<     // Signal handler for CTRL-C
<     signal(SIGINT, signalhandler);
<     signal(SIGTSTP, sigz);
<     
<     while(1)
---
> int ls(char **argv){
> 
>     if (argv[1] == NULL || ((strcmp(argv[1],"-a")==0) && argv[2] == NULL)) {
>         char tmp[10];
>         tmp[0] = '.'; tmp[1] = '\0';
>         list_dir(tmp, argv[1]);
>         return 1;
>     }
>     if (((strcmp(argv[1], "-l")==0) && (argv[2] == NULL)) || ((strcmp(argv[1], "-la")==0) && (argv[2] == NULL))
>      || ((strcmp(argv[1], "-al")==0) && (argv[2] == NULL))) {
>         char tmp[10];
>         tmp[0] = '.'; tmp[1] = '\0';
>         detailed_list_dir(tmp, argv[1]);
>         return 1;
>     }
>     if((argv[1][0] != '-' && argv[2] == NULL)|| ((strcmp(argv[1],"-a")==0) && argv[2][0] != '-' && argv[3] == NULL) || 
>         (argv[1][0]!='-' && (strcmp(argv[2],"-a")==0)  && argv[3] == NULL))
46,53c145,148
<         myline = read_line();
<         //if (strncmp(myline,"exit",4)==0)break;
<         arg_array = split_line(myline,";");
<         for(int i=0;arg_array[i];i++)
<         {    args = split_line(arg_array[i],NULL);
<             execute(args);
<         }
<         print_prompt();  
---
> 
>         if (argv[1][0] != '-') list_dir(argv[1], argv[2]);
>         else list_dir(argv[2], argv[1]);
>         return 1;
55c150,167
<     return 0;
---
> 
>     if (argv[1][0] != '-'){
>         if (((strcmp(argv[2], "-l")==0) && (argv[3] == NULL)) || ((strcmp(argv[2], "-la")==0) && (argv[3] == NULL))
>      || ((strcmp(argv[2], "-al")==0) && (argv[3] == NULL)))
>             detailed_list_dir(argv[1], argv[2]);
>         else  printf("No such command exists\n");
>         return 1;
>     }
>     if(argv[2]!=NULL && argv[2][0] != '-'){
>         if (((strcmp(argv[1], "-l")==0) && (argv[3] == NULL)) || ((strcmp(argv[1], "-la")==0) && (argv[3] == NULL))
>      || ((strcmp(argv[1], "-al")==0) && (argv[3] == NULL)))
>             detailed_list_dir(argv[2], argv[1]);
>         else  printf("No such command exists\n");
>         return 1;
>     }
>     
>     printf("No such command exists\n");
>     return 1;
