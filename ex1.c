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
        if (list[i].isInBackground && !list[i].isDone) {
            printf("%s\n", list[i].commandName);
        }
    }
    list[size].isDone = 1;
}

void history(struct Command list[100], int size) {
    int i = 0;
    int isLastCommand = 0;
    char commandNameCopy[100];
    for(; i <= size; i++) {
        if (i == size) {isLastCommand = 1;}
        strcpy(commandNameCopy, list[i].commandName);
        if (((waitpid(list[i].childPid, NULL, WNOHANG) == 0) || isLastCommand) && !list[i].isDone ) {
            printf("%s ", commandNameCopy);
            printf("RUNNING\n");
        } else {
            printf("%s ", commandNameCopy);
            printf("DONE\n");
        }
    }
    list[size].isDone = 1;
}


int numOfWords(char *string)
{
    int counter = 0 , i, len;
    char lastC;
    len = strlen(string);
    if(len > 0){
        lastC = string[0];
    }
    for(i = 0; i <= len; i++){
        if((string[i] == ' ' || string[i] == '\0') && lastC != ' '){
            counter++;
        }
        lastC = string[i];
    }
    return counter;
}


void cd(struct Command list[100], int size, char *lastPath) {
    char* homePath = getenv("HOME");
    char* newPath;
    char cdCommand[100];
    char currentPath[100];
    //char lastPath[100];
    strcpy(cdCommand, list[size].commandName);
    char* flags = cdCommand;
    flags += 3;

    if (getcwd(currentPath, sizeof(currentPath)) == NULL) {
        printf("An error occurred\n");
        return;
    }
    //int chdirStatus;

    //if there are too many flags
    if(2 < numOfWords(cdCommand)) {
        printf("Too many arguments\n");
        return;
    }
    // the command cd ~ comes alone without path
    if (strcmp(flags, "~") == 0 && strcmp(getLastWord(cdCommand), flags) == 0) {
        if (chdir(homePath) == -1) {
            printf("chdir failed\n");
            return;
        } else {
            return;
        }
    }
    if (strcmp(flags, "~") == 0) {
        // flags++;
        // newPath = strcat(homePath, flags);
        if (chdir(strcat(homePath, ++flags)) == -1) {
            printf("chdir failed\n");
            return;
        } else {
            return;
        }
    }
    if(chdir(flags) == -1){
        printf("chdir failed\n");
        return;
    } else {
        return;
    }
}

int main() {
    struct Command listOfCommands[100];
    int shellIsRunning = 1;
    char* firstWord;
    char input[100];
    char copyOfInput[100];
    int commandID = 0;
    char lastPath[100];
    pid_t pid;

    while(1) {
        printf("$ ");
        fflush(stdout);
        scanf(" %[^\n]", input);

        if (strcmp(getLastWord(input), "&") == 0) {
            listOfCommands[commandID].isInBackground = 1;
            input[strlen(input) - 1] = 0;
        } else {
            listOfCommands[commandID].isInBackground = 0;
        }
        strcpy(copyOfInput, input);
        strcpy(listOfCommands[commandID].commandName, input);
        listOfCommands[commandID].isDone = 0;
        listOfCommands[commandID].childPid = 0;
        firstWord = getFirstWord(input);
        // if command is a built in function
        if(strcmp(firstWord, "jobs") == 0  || strcmp(firstWord, "history") == 0
           || strcmp(firstWord, "cd") == 0 || strcmp(firstWord, "exit") == 0) {
            listOfCommands[commandID].isBuiltIn = 1;
            if(strcmp(firstWord, "jobs") == 0){
                jobs(listOfCommands, commandID);
            }
            if(strcmp(firstWord, "history") == 0){
                history(listOfCommands, commandID);
            }
            if(strcmp(firstWord, "cd") == 0){
                cd(listOfCommands, commandID, lastPath);
            }
            if(strcmp(firstWord, "exit") == 0){
                exitFromShell();
            }
            if(strcmp(firstWord, "echo") == 0) {
                char* echoInput;
                strcpy(echoInput, input);
                int i = 0;
                for (; i < strlen(echoInput); i++) {
                    if (strcmp(echoInput[i], '"') == 0) {
                        echoInput[i] = '\0';
                    }

                }

            }
            // if command is not a built in function
        } else {
            pid = fork();
            if (pid < 0) {
                printf("fork failed\n");
                fflush(stdout);
            }
            // if its the parent
            if (pid > 0) {
                listOfCommands[commandID].childPid = pid;
                // if the command doesnt suppose to be in background
                if(!listOfCommands[commandID].isInBackground) {
                    // wait to child process
                    if(waitpid(pid, NULL, 0) == -1) { // todo maybe bug here
                        printf("An error occured\n");
                        exitFromShell();
                    }
                }
            }
            // if its child process
            if (pid == 0) {
                // split the command into tokens
                char delim[] = " ";
                char *exeCommands[100];
                int i = 0;
                char *ptr = strtok(copyOfInput, delim);
                exeCommands[i] = ptr;
                while (ptr != NULL) {
                    ptr = strtok(NULL, delim);
                    i++;
                    exeCommands[i] = ptr;
                }
                exeCommands[i + 1] = NULL;
                // execute the command
                if (execvp(exeCommands[0], exeCommands) == -1) {
                    printf("exec failed\n");
                    fflush(stdout);
                    exitFromShell();
                }
            }
        }
        commandID++;
    }
    return 0;
}