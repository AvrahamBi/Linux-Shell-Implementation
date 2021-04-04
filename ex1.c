// Avraham Bar Ilan 205937949

#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


struct Command {
    char commandName[100];
    int isDone;
    int isInBackground;
    int isBuiltIn;
    pid_t childPid;

};

void exitFromShell(){
    exit(0);
}


char* getFirstWord(char* str){
    const char *delim = " ";
    char* firstWord = strtok(str, delim);
    return firstWord;
}

char* getLastWord(char* str){
    char* lastWord = strrchr(str, ' ');
    if (lastWord) {
        lastWord++;
        return lastWord;
    }
    return str;
}

void jobs(struct Command list[100], int size) {
    int i = 0;
    for(; i <= size; i++) {
        if(list[i].childPid == 0 || waitpid(list[i].childPid, NULL, WNOHANG) != 0) {
            list[i].isDone = 1;
        } else {
            list[i].isDone = 0; // todo maybe delete it
            if(list[i].isInBackground == 1) {
                printf("%s\n", list[i].commandName);
                fflush(stdout);
            }

        }
    }
}

void history(struct Command list[100], int size) {
    int i = 0;
    for(; i <= size; i++) {
        if(list[i].childPid == 0 || waitpid(list[i].childPid, NULL, WNOHANG) != 0) {
            printf("%s ", list[i].commandName);
            printf("DONE\n");
            fflush(stdout);
        } else {
            printf("%s ", list[i].commandName);
            printf("RUNNING\n");
        }
    }
}

int main() {
    struct Command listOfCommands[100];
    int shellIsRunning = 1;
    char* firstWord;
    char input[100];
    int commandID = 0;
    pid_t pid;

    while(shellIsRunning) {
        printf("$ ");
        fflush(stdout);
        scanf(" %[^\n]", input);

        if (strcmp(getLastWord(input), "&") == 0) {
            listOfCommands[commandID].isInBackground = 1;
            input[strlen(input) - 1] = 0;
        } else {
            listOfCommands[commandID].isInBackground = 0;
        }
        strcpy(listOfCommands[commandID].commandName, input);
        listOfCommands[commandID].isDone = 0;
        listOfCommands[commandID].childPid = 0;
        firstWord = getFirstWord(input);
        if(strcmp(firstWord, "jobs") == 0  || strcmp(firstWord, "history") == 0
            || strcmp(firstWord, "cd") == 0 || strcmp(firstWord, "exit") == 0) {
            listOfCommands[commandID].isBuiltIn = 1;
        }


        if(listOfCommands[commandID].isBuiltIn == 1) {
            if(strcmp(firstWord, "jobs") == 0){
                jobs(listOfCommands, commandID);
            }
            if(strcmp(firstWord, "history") == 0){
                history(listOfCommands, commandID);
            }
            if(strcmp(firstWord, "cd") == 0){

            }
            if(strcmp(firstWord, "exit") == 0){
                exitFromShell();
            }


        } else if(listOfCommands[commandID].isBuiltIn == 0) {
            pid = fork();
            if(pid < 0) {
                printf("fork failed\n");
                fflush(stdout);
            }
            listOfCommands[commandID].childPid = pid;



        }



        commandID++;
        shellIsRunning--;
    }
    return 0;
}