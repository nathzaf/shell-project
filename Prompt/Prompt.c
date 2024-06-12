#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>

#include "Prompt.h"
#include "../RaiseError/RaiseError.h"

/*
 * prompt() writes in the standard output "dash> " and then wait for a command to be type by the user
 * if there is an error while reading the input, an error will be raised and the programm will quit
 * if there is no error, the input will be stored in @read
 */
void prompt(char* read, size_t size){
    printf("dash> ");
    int success = getline(&read, &size, stdin);
    if(success == -1) raiseError(1);
    read[strcspn(read, "\n")] = 0; //deleting carriage return at the end
}