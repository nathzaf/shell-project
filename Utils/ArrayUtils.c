#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>

#include "ArrayUtils.h"

/*
 * splitToArray() takes a string @origin and a delimiter @delim and split the string to an array of string, separated by the delimiter, putting the result in @dest
 * for exemple "ls -l -n" with delimiter " " will be transformed to {"ls", "-l", "-n"}
 * it returns the size of the created array
 */
int splitToArray(char** dest, char* origin, char* delim){
    char* cpy = strdup(origin);
    int i = 0;
    do{
        dest[i] = strsep(&cpy, delim);
    } while(dest[i++] != NULL);
    return i - 1;
}

void printArray(char** array, int length){
    for(int i = 0; i < length; i++) printf("%s\n", array[i]);
}