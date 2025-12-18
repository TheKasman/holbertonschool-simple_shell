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
int execute(char **tokens, char *prog, int count, int *last_status);
void run_command(char *path, char **tokens);
int search_path(char *cmd, char **tokens, char *prog, int count);
char *get_env_var(const char *name);
void print_env(void);


/*TO DO*/
void do_cd(char **tokens);
/*void print_env(char **envp);*/

#endif
