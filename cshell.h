#define TOKKEN_BUFFERSIZE 64
//#define TOK_DELIM " \t\r\n\a"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

extern int sigsize;
typedef struct process{
	int  num;
	char pname[50];
    int state;
}process;

struct passwd * myname;
char hostname[30];
char * curname;
process * proc[200];

int foreground_id;

char shellid[10];

void sigz(int signum);
int pinfo_id(char ** args);
int print_details(char *filepath);
void trunc_line(char * str);
int piping(char ** args);

char ** redirect(char ** args);
void sigh(int signum);
    
void print_prompt();
char * itoa(int num , char * pid);
char * pinfostate(int pid);

int overkill(char **args);

char myhome[1024];

char **split_line(char *line,char * delim);

char * read_line();

// int change_dir(char **args);

int cd(char **args);
char* home;

// int checkCd(char* command, char initialdir);

int echo(char ** args);
int jobs(char **args);
int kjob(char **args);
int bg(char **args);
int fg(char **args);
int pwd();



int launch(char **args);
void list_dir(char **argv);

void detailed_list_dir(char **argv);


int ls(char **argv);

int exit_shell(char ** argv);


int execute(char ** argv);

void signalhandler();

int setVar(char **args);

int unsetVar(char **args);

int main(int argc, char const *argv[]);
