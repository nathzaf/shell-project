#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RaiseError/RaiseError.h"
#include "Utils/ArrayUtils.h"
#include "Utils/StringUtils.h"
#include "Execution/Execution.h"
#include "Prompt/Prompt.h"

#define EVER ;;
#define UNUSED(x) (void)(x)

char* path; //global variable containing the path(s) in a string
char* pathArray[50]; //global variable containing all paths in an array of string
int pathCounter;
int redirection; //global state variable indicating if the current command ask for a redirection
int exitDash; //global state variable indicating if the type command is 'exit'
int error; //global state variable indicating if an error has occurred

int main(int argc, char* argv[]){
    UNUSED(argv); //muting unused parameter warning
    exitDash = 0;
    if(argc > 1) raiseError(1);

    path = "/bin/"; //setting default path
    printf("Starting dash with path %s\n", path);

    int argsCounter;
    char* args[50];
    for(int i = 0; i < 50; i++){
        pathArray[i] = malloc(50 * sizeof(char));
        args[i] = malloc(50 * sizeof(char));
    }
    char* read = malloc(50 * sizeof(char));
    char* readWithRedirectionSymbol = malloc(50 * sizeof(char));
    char* readCleared = malloc(50 * sizeof(char));

    pathCounter = splitToArray(pathArray, path, " ");

    for(EVER){
        redirection = 0;
        error = 0;
        prompt(read, 40);

        if(charOccurrence(read, '<') > 1){
            raiseError(0);
            continue;
        }else if(charOccurrence(read, '<') == 1){
            redirection = 1; //setting global state variable redirection to 1 to notify that there is a redirection so that we don't need the '<' anymore

            readWithRedirectionSymbol = strdup(read); //we save a version of the string with <
            readWithRedirectionSymbol += strcspn(read, "<") + 1; //pointing one char after < to only have the arguments after <
            deleteWhitespaces(readWithRedirectionSymbol, readCleared);
            if(splitToArray(args, readCleared, " ") > 1){ //if split gives strictly more than 1 string, then there are more than one argument after <, it's an error
                raiseError(0);
                continue;
            }

            read[strcspn(read, "<")] = ' '; //replacing the < by a space to handle the case where there is no spaces around him (so the command and the file name will be separated), multiple spaces will be deleted by deleteWhitespaces()
        }

        deleteWhitespaces(read, readCleared);
        argsCounter = splitToArray(args, readCleared, " ");

        if(strcmp(args[0], "exit") == 0){
            free(read);
            free(readCleared);
            free(readWithRedirectionSymbol);
            for(int i = 0; i < pathCounter; i++) free(pathArray[i]);
            for(int i = 0; i < argsCounter; i++) free(args[i]);
            exitDash = 1;
        }

        dashExecute(args, argsCounter);
    }
}