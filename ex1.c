// Avraham Bar Ilan 205937949


#include <stdio.h>
#include <memory.h>
#include<unistd.h>




int main() {
    int stop = 0;
    char arg[100];
    char listOfWords[100][100];
    char delim[] = " ";
    while(!stop) {
        printf("$ ");
        scanf(" %[^\n]s", arg);
        char *ptr = strtok(arg, delim);
        int i = 0;
        while (ptr != NULL) {
            strcpy(listOfWords[i], ptr);
            ptr = strtok(NULL, delim);
            i++;
        }
        int numOfArgs = i;
        for (int j = 0; j < numOfArgs; j++) {
            printf("%s\n", listOfWords[j]);
        }

        if(strcmp(arg, "kill") == 0) stop = 1;

    }


    //execlp("man", "man", "man", NULL);

    return 0;
}