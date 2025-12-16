#ifndef HSH_H
#define HSH_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

extern char **environ;

char *read_input(FILE *stream);
char **parse(char *input);
void execute(char **tokens, char *prog, int count);
void run_command(char *path, char **tokens);
void search_path(char *cmd, char **tokens, char *prog, int count);


/*TO DO*/
void do_cd(char **tokens);
void print_env(char **envp);
#endif
