// Avraham Bar Ilan 205937949

#include <stdio.h>
#include <memory.h>
#include <unistd.h>

int main() {

    /*commands[0] = "man";
    commands[1] = "man";
    commands[2] = NULL;
    printf("$\n");
    char temp;

    // clear the buffer
    scanf("%c", &temp);
    //fflush(stdout);
    int i = 0;
    while(scanf("%[^\n]s", commands[i]) != NULL) {
        printf("%s\n", commands[i]);
        i++;
    }*/
    /*scanf("%[^\n]s", commands);
    char *firstCommand = strtok(commands, " ");*/

   // execvp(firstCommand, commands);




   char *commands[100];
   char *temp;
   int i= 0;
   char input[100];
   fflush(stdout);
   scanf(" [^\n]%s", input);
   temp = strtok(input, " ");
   while( temp != NULL){
       commands[i] = temp;
       temp = strtok(NULL, " ");
       i++;
   }
   printf("%s ", commands[0]);
   printf("%s", commands[1]);
   return 0;
}