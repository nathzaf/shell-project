#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "RaiseError.h"

extern int error;

/*
 * raiseError() writes in the standard error output the only error message possible in this program
 * If it is called with non-zero integer, it will exit with a failure status
 * Finally, it put the global state variable to 1 to notify an error (for unit testing)
*/
void raiseError(int c){
    error = 1;
    char errorMessage[30] = "An error has occurred\n";
    write(STDERR_FILENO, errorMessage, strlen(errorMessage));
    if(c != 0) exit(EXIT_FAILURE);
}