// Avraham Bar Ilan 205937949

#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


struct Command {
    char commandName[100];
    char firstCommand[100];
    int isDone;
    int IsInBackground;
    pid_t childPid;

};

void exitWin(){
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

int main() {
    struct Command listOfCommands[100];
    int shellIsRunning = 1;
    char input[100];
    while(shellIsRunning) {
        printf("$ ");
        fflush(stdout);
        scanf(" %[^\n]", input);








        shellIsRunning--;
    }
    return 0;
}