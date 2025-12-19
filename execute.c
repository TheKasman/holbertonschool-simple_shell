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
 * print_env - Prints all environment variable per line
 */
void print_env(void)
{
	int i = 0;

	if (!environ)
		return;

	while (environ[i])
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

/**
 * run_command - a helper function to run the command at a given path
 * @path: the path
 * @tokens: our tokens
 * Return: returns the correct exitstatus for the child
 */

int run_command(char *path, char **tokens)
{
	pid_t pid = fork();
	int status;

	if (pid == 0)
	{
		execve(path, tokens, environ);
		perror("execve");
		exit(127);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	else
		perror("fork");
	return (1);
}

/**
 * search_path - searches the path for command, and runs it
 * @cmd: the command
 * @tokens: our tokens
 * @prog: program name
 * @count: cmd count
 * Return: returns an exit code
 */

int search_path(char *cmd, char **tokens, char *prog, int count)
{
	char *path = get_env_var("PATH"), *copy, *dir;
	char full_path[1024];
	int status;

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
			status = run_command(full_path, tokens);
			free(copy);
			return (status); /*we did it!*/
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
 * @last_status: the status of the child program
 * Return: returns the status code
 */

int execute(char **tokens, char *prog, int count, int *last_status)
{
	char *cmd = tokens[0];
	int status; /*consumes return from search_path*/

	if (!cmd || !prog)
	{
		*last_status = 127;
		return (127);
	}
	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == -1)
		{
			fprintf(stderr, "%s: %d: %s: not found\n", prog, count, cmd);
			status = 127;
		}
		else
		{
			status = run_command(cmd, tokens);
		}
		*last_status = status;
	}
	else
	{
		status = search_path(cmd, tokens, prog, count);
		*last_status = status;
	}
	return (status);
}
