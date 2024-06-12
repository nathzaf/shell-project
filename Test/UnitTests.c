#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>

#include "minunit.h"
#include "../Utils/ArrayUtils.h"
#include "../Utils/StringUtils.h"
#include "../Execution/Execution.h"

char* path;
char* pathArray[50];
int pathCounter;
int redirection;
int exitDash;
int error;

char* helloWorld = "Hello world !";

void test_setup(void){
    error = 0;
    path = "/bin/";
    pathArray[0] = "/bin/";
    pathCounter = 1;
    redirection = 0;
    exitDash = 0;
}

void test_teardown(void){
    /* Nothing */
}

MU_TEST(test_deleteWhitespaces){
    char* str = "  \t   Hello     world  \t\t    !  \t \t   \t";
    char* res = malloc(strlen(str) * sizeof(char) + 1);
    deleteWhitespaces(str, res);
    mu_check(strcmp(res, helloWorld) == 0);
    free(res);
}

MU_TEST(test_deleteWhitespaces_2){
    char* res = malloc(strlen(helloWorld) * sizeof(char) + 1);
    deleteWhitespaces(helloWorld, res);
    mu_check(strcmp(res, helloWorld) == 0);
    free(res);
}

MU_TEST(test_deleteWhitespaces_3){
    char* str = "\t\t \t  ls   \t  -l  \t";
    char* res = malloc(strlen(helloWorld) * sizeof(char) + 1);
    char* expected = "ls -l";
    deleteWhitespaces(str, res);
    mu_check(strcmp(res, expected) == 0);
    free(res);
}

MU_TEST(test_splitToArray){
    int length = 3;

    char** strList = malloc(length * sizeof(char*));
    char** expected = malloc(length * sizeof(char*));

    for(int i = 0; i < length; i++) expected[i] = malloc(10 * sizeof(char));

    int lengthResult = splitToArray(strList, helloWorld, " ");

    strcpy(expected[0], "Hello");
    strcpy(expected[1], "world");
    strcpy(expected[2], "!");

    for(int i = 0; i < length; i++) mu_check(strcmp(strList[i], expected[i]) == 0);
    mu_assert_int_eq(lengthResult, length);

    for(int i = 0; i < length; i++){
        free(expected[i]);
    }
    free(strList);
}

MU_TEST(test_charOccurence){
    mu_assert_int_eq(charOccurrence(helloWorld, 'l'), 3);
    mu_assert_int_eq(charOccurrence(helloWorld, 'a'), 0);
    mu_assert_int_eq(charOccurrence("wc -l < text.txt", '<'), 1);
}

MU_TEST(test_command_with_no_path){
    path = "";
    pathArray[0] = "";
    pathCounter = 0;

    char** command = malloc(2 * sizeof(char*));
    command[0] = malloc(3 * sizeof(char));
    command[1] = malloc(3 * sizeof(char));
    strcpy(command[0], "ls");
    strcpy(command[1], "-l");

    dashExecute(command, 2);

    mu_assert_int_eq(error, 1);

    free(command[0]);
    free(command[1]);
}

MU_TEST(test_command_with_wrong_path){
    path = "/coucou/";
    pathArray[0] = "/coucou/";
    pathCounter = 1;

    char** command = malloc(2 * sizeof(char*));
    command[0] = malloc(3 * sizeof(char));
    command[1] = malloc(3 * sizeof(char));
    strcpy(command[0], "ls");
    strcpy(command[1], "-l");

    dashExecute(command, 2);

    mu_assert_int_eq(error, 1);

    free(command[0]);
    free(command[1]);
}

MU_TEST(test_inexisting_command){
    char** command = malloc(2 * sizeof(char*));
    command[0] = malloc(3 * sizeof(char));
    command[1] = malloc(3 * sizeof(char));
    strcpy(command[0], "a");
    strcpy(command[1], "-l");

    dashExecute(command, 2);

    mu_assert_int_eq(error, 1);

    free(command[0]);
    free(command[1]);
}

MU_TEST(test_command_no_redirection){
    char** command = malloc(2 * sizeof(char*));
    command[0] = malloc(3 * sizeof(char));
    command[1] = malloc(3 * sizeof(char));
    strcpy(command[0], "ls");
    strcpy(command[1], "-l");

    dashExecute(command, 2);
    mu_assert_int_eq(error, 0);

    strcpy(command[0], "ps");
    strcpy(command[1], "-n");
    dashExecute(command, 2);
    mu_assert_int_eq(error, 0);

    free(command[0]);
    free(command[1]);
}

MU_TEST(test_command_redirection){
    redirection = 1;

    char** command = malloc(4 * sizeof(char*));
    for(int i = 0; i < 3; i++) command[i] = malloc(20 * sizeof(char));

    strcpy(command[0], "wc");
    strcpy(command[1], "-l");
    strcpy(command[2], "Test/testFile.txt");

    dashExecute(command, 3);

    mu_assert_int_eq(error, 0);

    for(int i = 0; i < 4; i++) free(command[i]);
}


MU_TEST_SUITE(test_suite){
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_deleteWhitespaces);
    MU_RUN_TEST(test_deleteWhitespaces_2);
    MU_RUN_TEST(test_deleteWhitespaces_3);

    MU_RUN_TEST(test_splitToArray);

    MU_RUN_TEST(test_charOccurence);

    MU_RUN_TEST(test_command_with_no_path);
    MU_RUN_TEST(test_command_with_wrong_path);
    MU_RUN_TEST(test_inexisting_command);
    MU_RUN_TEST(test_command_no_redirection);
    MU_RUN_TEST(test_command_redirection);

}

int main(){
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
