#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>

#define BUFFER_SIZE 2048

char directory[BUFFER_SIZE];
int pid;

void help(){
    printf("Autor: Daniel Strzelbicki\nKomendy: exit, ls, cd, pwd\n");
}

void pwd(){
    getcwd(directory, 2048);
    printf("%s\n", directory);
}

void printPath(){
    getcwd(directory, 2048);
    printf("\n[%s] $ ", directory);
}

void cd(char * str){
    if(str == NULL){
        printf("No arguments\n");
        return;
    }
    if(strcmp(str, "~")==0){
        chdir(getenv("HOME"));
    }else if(chdir(str)<0){
        printf("Failed\n");
    }
}

void ls(){
    DIR *d;
    struct dirent *dir;
    d = opendir(directory);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
}

char *trim(char *str){
    char *end;
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0){
        return str;
    }

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';

    return str;
}

int main(int argc, char* argv[]){
    char input[BUFFER_SIZE];
    char *commands[BUFFER_SIZE];
    char *command[BUFFER_SIZE];
    char *saveToFileStream[BUFFER_SIZE];
    char *line;

    while(1){
        printPath();
        fgets(input,sizeof(input),stdin);
        int i=0;
        line = strtok(input,"\n");
        char *tmp1 = strtok(line, "|");
        while (tmp1 != NULL){
            commands[i] = tmp1;
            commands[i] = trim(commands[i]);
            tmp1 = strtok(NULL, "|");
            i++;
        }

        while (i--){
            int j=0;
            char *tmp2 = strtok(commands[i], " ");
            while (tmp2 != NULL){
                command[j] = tmp2;
                command[j] = trim(command[j]);
                tmp2 = strtok(NULL, " ");
                j++;
            }

        if(!strcmp(command[0],"help")){
            help();
        }else if(!strcmp(command[0],"cd")){
            cd(command[1]);
        }else if(!strcmp(command[0],"ls")){
            ls();
        }else if(!strcmp(command[0],"pwd")){
            pwd();
        }else if(!strcmp(command[0],"exit")){
            exit(1);
        }else{
            pid = fork();
            if(pid <0){
            printf("Failed to fork");
            exit(1);
        }else if(pid == 0){
            if(execvp(command[0], &command[0])<0){
                printf("Failed");
                exit(1);
            }
        }
        }
    }
    }
    return 0;
}
