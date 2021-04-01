/*int stop = 0;
    char command[100];
    char delim[] = " ";
    while(!stop) {
        printf("$ ");
        //fflush(stdout);
        scanf(" %[^\n]s", command);
        char *ptr = strtok(command, delim);
        int i = 0;
        while (ptr != NULL) {
            strcpy(listOfWords[i], ptr);
            ptr = strtok(NULL, delim);
            i++;
        }

        char *execArgs[100];
        for (int j = 0; j < i; j++) {
            strcpy(*execArgs[100], listOfWords[j]);
        }
        printf("%s", execArgs[1]);
        //execvp("echo", execArgs);


        //execvp(listOfWords[0], listOfWords);

        if(strcmp(args, "kill") == 0) stop = 1;



    }


    //execlp("man", "man", "man", NULL);
*/