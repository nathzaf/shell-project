#define _GNU_SOURCE

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "Execution.h"
#include "../RaiseError/RaiseError.h"
#include "../Utils/ArrayUtils.h"

extern char* path;
extern char* pathArray[50];
extern int pathCounter;
extern int redirection;
extern int exitDash;

/*
 * dashExecute() is one of the main function of this program
 * it takes @args, an array of string that contains at the first position @args[0] the command to execute
 * Case 1 : "exit" -> it will exit with a success status
 * Case 2 : "cd" -> it will change the current directory
 * Case 3 : "path" -> it will change the path for execution
 * Case 4 : anything else -> it will try to execute the command with all paths available in @pathArray, if the command is executable with one path, then it will call commandExecute() to execute it
 * if the command can not be executed by any of the paths, it will raise and error without exiting
 */
void dashExecute(char** args, int argsCounter){
    if(exitDash){ //using global state variable exit instead of a strcmp because of free in main
        if(argsCounter > 1) raiseError(0);
        else exit(EXIT_SUCCESS);
    }else if(strcmp(args[0], "cd") == 0){
        if(argsCounter == 1 || argsCounter > 2) raiseError(0);
        else{
            if(chdir(args[1]) == -1) raiseError(0);
        }
    }else if(strcmp(args[0], "path") == 0){
        if(argsCounter == 1){
            path = "";
            pathCounter = 0;
            strcpy(pathArray[0], "");
            pathArray[1] = '\0';
        }else{
            path = strdup(args[1]);
            for(int i = 2; i < argsCounter; i++){
                strcat(path, " ");
                strcat(path, strdup(args[i]));
            }
            pathCounter = splitToArray(pathArray, path, " ");
            printf("%d new path(s) set to : \n", pathCounter);
            printArray(pathArray, pathCounter);
        }
    }else{
        char* command;
        int try = 0;
        for(int i = 0; i < pathCounter; i++){ //trying command with all paths until one works
            command = strdup(pathArray[i]);
            strcat(command, args[0]);
            if(access(command, X_OK) == -1){
                try++;
            }else{
                if(redirection == 0){
                    commandExecute(command, args);
                    break;
                }else{
                    char* fileName = strdup(args[argsCounter - 1]); //geting name of the file to open
                    args[argsCounter - 1] = 0; //deleting file name from command
                    commandExecuteWithRedirection(command, args, fileName);
                    break;
                }
            }
        }
        if(try == pathCounter) raiseError(0);
    }
}

/*
 * This function will fork, execute the command in the child process and make the parent process wait for the child
 */
void commandExecute(char* command, char** args){
    pid_t rc = fork();
    if(rc == -1) raiseError(0);
    else if(rc == 0){ // CHILD
        if(execv(command, args) == -1) raiseError(1);
    }else{ // PARENT
        wait(NULL);
    }
}

/*
 * This does the same as commandExecute, but open a file and make a redirection of the standard input
 */
void commandExecuteWithRedirection(char* command, char** args, char* fileName){
    pid_t rc = fork();
    if(rc == -1) raiseError(0);
    else if(rc == 0){ // CHILD
        int fd = open(fileName, O_RDONLY);
        if(fd < 0) raiseError(1); //if we raiseError(0) the child process will continue to live and dash will be duplicated, and for example an exit will just exit the child but not the parent
        else{
            if(dup2(fd, STDIN_FILENO) == -1) raiseError(1);
            else{
                if(execv(command, args) == -1){
                    raiseError(0);
                }
            }
            if(close(fd) == -1) raiseError(1);
        }
    }else{ // PARENT
        wait(NULL);
    }
}