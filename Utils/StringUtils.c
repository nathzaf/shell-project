#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>

#include "StringUtils.h"

/*
 * deleteWhitespaces() deletes tabulations and multiple spaces in a string
 * for exemple "   \t   ls  \t    -l      \t" will be transformed to "ls -l"
 */
void deleteWhitespaces(char* str, char* res){
    int i = 0;
    int j = 0;
    char buf[50];
    while(str[i] != '\0'){
        if(str[i] != '\t'){ //deleting all tabulation first
            buf[j] = str[i];
            j++;
        }
        i++;
    }
    buf[j] = '\0';

    i = 0;
    j = 0;
    while(buf[i] != '\0'){
        if((buf[i] == ' ' && buf[i + 1] == ' ') != 1){ //then deleting >=2 spaces in a row
            res[j] = buf[i];
            j++;
        }
        i++;
    }
    res[j] = '\0';

    for(int k = j - 1; res[k] == ' '; k--) res[k] = '\0'; // deleting spaces at the end;

    if(res[0] == ' '){ // deleting first space if present
        for(int k = 1; k <= j; k++){
            res[k - 1] = res[k];
        }
    }
}

/*
 * this function count the number if occurrence of a char @c in a string @str
 */
int charOccurrence(char* str, char c){
    int cpt = 0;
    for(int i = 0; i < (int) strlen(str); i++){
        if(str[i] == c) cpt++;
    }
    return cpt;
}