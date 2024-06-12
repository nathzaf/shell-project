#ifndef EXECUTION_H
#define EXECUTION_H

void dashExecute(char** args, int argsCounter);

void commandExecute(char* command, char** args);

void commandExecuteWithRedirection(char* command, char** args, char* fileName);

#endif
