/* 
Team Platinum
Zeyu Ji
Huan Ying
10/27/2020
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_LINE 1024
char **get_input(char *); // get input
int cd(char *); // cd command
int userCmd(char**) ;
int userCmdct(char**);
void help() ;
void hello();
void readFile(char*);
void writeFile(char*);

int main() {
    char **command;
    char *input;
    pid_t child_pid;
    int stat_loc;
    hello();

    while (1) {
        input = readline("Myshell> ");
        command = get_input(input);
        if (!command[0]) {      // Handle empty commands
            free(input);
            free(command);
            continue; // if it's empty, continue loop
        }
	if (userCmd(command)==1){
	free(input);
     	free(command);
	continue;
	}
        

        child_pid = fork(); //fork a child thread
        if (child_pid < 0) { // if os don't have enough memory or reach the maximum threads
            perror("Fork failed");
            exit(1);
        }
        if (child_pid == 0) { // for child thread, run execvp(), and usercommand need child thread
            if (userCmdct(command)==1){
            	free(input);
            	free(command);
            	exit(0);
	        continue;
	     }
            execvp(command[0], command);
            exit(0);
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }

        free(input);
        free(command);
    }

    return 0;
}

char **get_input(char *input) {
    char **cmd = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL) {
        cmd[index] = parsed;
        index++;
        parsed = strtok(NULL, separator);
    }

    cmd[index] = NULL;
    return cmd;
}
int userCmdct(char** cmd){ // for user commands need child thread
int count = 4, i, switchArg = 0; 
    char* userCmd[count]; 
    char *fn;
    userCmd[0] = "rf"; 
    userCmd[1] = "wf"; 
	for (i = 0; i < count; i++) { 
        if (strcmp(cmd[0], userCmd[i]) == 0) { 
            switchArg = i + 1; 
            break; 
        } 
    } 
    switch (switchArg) { 
    case 1: 
        fn = cmd[1];
        if (cmd[1]==NULL)
            fn = readline("File Name> ");
        readFile(fn);
        return 1;
    case 2: 
        fn = cmd[1];
        if (cmd[1]==NULL)
            fn = readline("File Name> ");
        writeFile(fn);
        return 1;
    default: 
        break; 
    } 
    return 0; 
}
void readFile(char* fn){
    char buf[MAX_LINE];
    int count = 1;
    FILE *fp = NULL;
    int len;
    if((fp =fopen(fn,"r")) == NULL){
        perror("fail to read!!!");
        exit(1) ;
}
        while(fgets(buf,MAX_LINE,fp) != NULL){
        	len= strlen(buf);
        	buf[len-1] = '\0'; //remove'/n'
        	printf("%d	%s	,count:%d \n",count,buf,len - 1);
        	count += 1;
        }
  
  return;
}

void writeFile(char* fn){
   char buf[MAX_LINE];
   FILE *fp =NULL;
   fp = fopen(fn, "w");
   if(fp == NULL){
      printf("fail to write!!!");
      exit(1);
   }
   printf("Enter String> ");
   fgets(buf, (sizeof buf / sizeof buf[0]), stdin);
   fprintf(fp,"%s", buf);
   fclose(fp);
   return;
}
int userCmd(char** cmd) {
 
    int count = 4, i, switchArg = 0; 
    char* userCmd[count]; 
  
    userCmd[0] = "exit"; 
    userCmd[1] = "cd"; 
    userCmd[2] = "help"; 
    userCmd[3] = "hello"; 
  
    for (i = 0; i < count; i++) { 
        if (strcmp(cmd[0], userCmd[i]) == 0) { 
            switchArg = i + 1; 
            break; 
        } 
    } 
  
    switch (switchArg) { 
    case 1: 
        printf("exit success\n");
        exit(0);
    case 2: 
        if (cd(cmd[1]) < 0) { //if it's not a dir error msg
                perror(cmd[1]);
            }
        return 1;
    case 3: 
        help(); 
        return 1; 
    case 4: 
        hello();
        return 1; 
    default: 
        break; 
    } 
    return 0; 
}
void help() { 
    printf("\n"
        "\n>cd - Change directory"
        "\n>pwd - Present Working directory"
        "\n>mkdir - Make a directory (Alerts if already exists)"
        "\n>rmdir- Remove the directory (Alerts if no such file or directory)"
        "\n>ls - List contents of pwd"
        "\n>cp - Copy contents from one file to another"
        "\n>rf - readfile - read a file and print it"
        "\n>wf - writefile - write to a file and print it"
        "\n>exit\n\n"
        ); 
  
    return; 
} 
void hello(){
	printf("\n***Hello HERE IS A SIMPLE SHELL***\n"
            "***Use help to know more..\n"
            ); 
    return;
}
int cd(char *path) {
    return chdir(path);
}
