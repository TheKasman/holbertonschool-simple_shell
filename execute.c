#include "hsh.h"

/**
 * get_env_var - gets the environment variable (our way to bypass getenv)
 * @name: the object to receive variables from
 * Return: returns the environment varibles
 */

char *get_env_var(const char *name)
{
	int i;
	size_t len = strlen(name);

	for (i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
		{
			return (environ[i] + len + 1);
		}
	}
	return (NULL);
}

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
 * @prog: program name
 * @count: cmd count
 */

int search_path(char *cmd, char **tokens, char *prog, int count)
{
	char *path = get_env_var("PATH"), *copy, *dir;
	char full_path[1024];

	if (!prog)
		return (1);

	if (!path)
	{
		fprintf(stderr, "%s: %d: %s: not found\n", prog, count, cmd);
		return (127); /*command not found*/
	}
	copy = strdup(path);
	if (!copy)
		return (1);

	for (dir = strtok(copy, ":"); dir; dir = strtok(NULL, ":"))
	{
		if (*dir == '\0')
			dir = ".";
		snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
		if (access(full_path, X_OK) == 0)
		{
			run_command(full_path, tokens);
			free(copy);
			return (0); /*we did it!*/
		}
	}
	free(copy);
	fprintf(stderr, "%s: %d: %s: not found\n", prog, count, cmd);
	return (127);
}

/**
 * execute - main execute function, handles external commands & forks & execve
 * @tokens: our tokens
 * @prog: program name
 * @count: cmd count
 */

void execute(char **tokens, char *prog, int count)
{
	char *cmd = tokens[0];
	int status; /*consumes return from search_path*/

	if (!cmd || !prog)
		return;

	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == -1)
		{
			fprintf(stderr, "%s: %d: %s: not found\n", prog, count, cmd);
			status = 127;
		}
		else
		{
			run_command(cmd, tokens);
			status = 0;
		}
	}
	else
		status = search_path(cmd, tokens, prog, count);

	/*optional for later: last_status = status*/
}
