// Avraham Bar Ilan

#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// the Command struct
struct Command {
    char commandName[100];
    int isDone;
    int isInBackground;
    int isBuiltIn;
    pid_t childPid;

};
// exit from shell built in function
void exitFromShell(){
    exit(0);
}
/* function that get a string and returns
    the first word of it
*/
char* getFirstWord(char* str){
    const char *delim = " ";
    char* firstWord = strtok(str, delim);
    return firstWord;
}
/* function that get a string and returns
    the last word of it
*/
char* getLastWord(char* str){
    char* lastWord = strrchr(str, ' ');
    if (lastWord) {
        lastWord++;
        return lastWord;
    }
    return str;
}
/* built in function that prints all the running tasks
*/
void jobs(struct Command list[100], int size) {
    int i = 0;
    // iterates over the command list
    for(; i <= size; i++) {
        // if a task is in background and still running
        if (list[i].isInBackground && !list[i].isDone) {
            printf("%s\n", list[i].commandName);
        }
    }
    list[size].isDone = 1;
}
// function that prints all the commands from the
// beginning of the shell run
void history(struct Command list[100], int size) {
    int i = 0;
    int isLastCommand = 0;
    char commandNameCopy[100];
    // iterate over the commands list
    for(; i <= size; i++) {
        if (i == size) {isLastCommand = 1;}
        strcpy(commandNameCopy, list[i].commandName);
        // if the task is still running
        if (((waitpid(list[i].childPid, NULL, WNOHANG) == 0) || isLastCommand) && !list[i].isDone ) {
            printf("%s ", commandNameCopy);
            printf("RUNNING\n");
        } else {
            // is task is done
            printf("%s ", commandNameCopy);
            printf("DONE\n");
        }
    }
    list[size].isDone = 1;
}

// function that returns how many
// words in a string
int numOfWords(char *string)
{
    int counter = 0 , i, len;
    char lastC;
    len = strlen(string);
    if(len > 0){
        lastC = string[0];
    }
    // iterates over the string
    for(i = 0; i <= len; i++){
        // if the char is space or null
        if((string[i] == ' ' || string[i] == '\0') && lastC != ' '){
            counter++;
        }
        lastC = string[i];
    }
    return counter;
}

// a built in function that take the job of the cd command
void cd(struct Command list[100], int size, char *lastPath) {
    char* homePath = getenv("HOME");
    char* newPath;
    char cdCommand[100];
    char currentPath[100];
    // copy the name of the command to prevent problems
    // while changing the name
    strcpy(cdCommand, list[size].commandName);
    char* flags = cdCommand;
    // make the pointer points to the first flag
    // of the command, and skip cd
    flags += 3;

    if (getcwd(currentPath, sizeof(currentPath)) == NULL) {
        printf("An error occurred\n");
        return;
    }
    int sizeOfCommand = numOfWords(cdCommand);
    //if there are too many flags
    if(2 < sizeOfCommand) {
        printf("Too many arguments\n");
        return;
    }
    // if command is only cd
    if (1 == sizeOfCommand) {
        if (chdir(homePath) == -1) {
            printf("chdir failed\n");
            return;
        } else {
            strcpy(lastPath, currentPath);
            return;
        }
    }
    // if command is only cd -
    if (strcmp(flags, "-") == 0 && strcmp(getLastWord(cdCommand), flags) == 0) {
        if (chdir(lastPath) == -1) {
            printf("chdir failed\n");
            return;
        } else {
            strcpy(lastPath, currentPath);
            return;
        }
    }
    // the command cd ~ comes alone without path
    if (strcmp(flags, "~") == 0 && strcmp(getLastWord(cdCommand), flags) == 0) {
        if (chdir(homePath) == -1) {
            printf("chdir failed\n");
            return;
        } else {
            strcpy(lastPath, currentPath);
            return;
        }
    }
    // if flag is ~
    if (strcmp(flags, "~") == 0) {
        if (chdir(strcat(homePath, ++flags)) == -1) {
            printf("chdir failed\n");
            return;
        } else {
            strcpy(lastPath, currentPath);
            return;
        }
    }
    if(chdir(flags) == -1){
        printf("chdir failed\n");
        return;
    } else {
        strcpy(lastPath, currentPath);
        return;
    }
}

int main() {
    struct Command listOfCommands[100];
    int shellIsRunning = 1;
    char* firstWord;
    char input[100];
    char copyOfInput[100];
    char echoInput[100];
    char copyForFirstWord[100];
    int commandID = 0;
    char lastPath[100];
    pid_t pid;

    // the loop of the shell
    while(1) {
        printf("$ ");
        fflush(stdout);
        scanf(" %[^\n]", input);
        strcpy(copyForFirstWord, input);
        firstWord = getFirstWord(copyForFirstWord);
        // if its echo remove the "" and the ''
        if(strcmp(firstWord, "echo") == 0) {
            int i = 0;
            int counter = 0;
            for (; i < strlen(input); i++) {
                char currentChar;
                currentChar = input[i];
                if (currentChar != '"' && currentChar != 39) { // 39 is the ascii value of '
                    echoInput[counter] = currentChar;
                    counter++;
                }

            }
            strcpy(input, echoInput);
        }
        // if the command meant to be in background
        if (strcmp(getLastWord(input), "&") == 0) {
            listOfCommands[commandID].isInBackground = 1;
            input[strlen(input) - 2] = '\0';
        } else {
            listOfCommands[commandID].isInBackground = 0;
        }
        strcpy(copyOfInput, input);
        strcpy(listOfCommands[commandID].commandName, input);
        listOfCommands[commandID].isDone = 0;
        listOfCommands[commandID].childPid = 0;
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