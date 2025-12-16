#include "hsh.h"

/**
 * run_command - a helper function to run the command at a given path
 * @path: the path
 * @tokens: our tokens
 */

void run_command(char *path, char **tokens)
{
	pid_t pid = fork();
	int status;

	if (pid == 0)
	{
		execve(path, tokens, environ);
		perror(tokens[0]);
		exit(1);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		perror("fork");
}

/**
 * search_path - searches the path for command, and runs it
 * @cmd: the command
 * @tokens: our tokens
 */

void search_path(char *cmd, char **tokens)
{
	char *path = getenv("PATH"), *copy, *dir;
	char full_path[1024];

	if (!path)
	{
		fprintf(stderr, "%s: command not found\n", cmd);
		return;
	}
	copy = strdup(path);
	if (!copy)
		return;

	for (dir = strtok(copy, ":"); dir; dir = strtok(NULL, ":"))
	{
		if (*dir == '\0')
			dir = ".";
		snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
		if (access(full_path, X_OK) == 0)
		{
			run_command(full_path, tokens);
			free(copy);
			return;
		}
	}
	free(copy);
	fprintf(stderr, "%s: command not found\n", cmd);
}

/**
 * execute - main execute function, handles external commands & forks & execve
 * @tokens: our tokens
 */

void execute(char **tokens)
{
	char *cmd = tokens[0];

	if (!cmd)
		return;

	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == -1)
			perror(cmd);
		else
			run_command(cmd, tokens);
		return;
	}
	search_path(cmd, tokens);
}
