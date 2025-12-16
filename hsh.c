#include "hsh.h"

#define MAX_TOKENS 64 /*maximum number of command arguments*/

/**
 * **parse - parses the input stream from read_input into usable content
 * @input: self explanatory.. it's the input.
 * Return: returns the input as separate tokens
 */

char **parse(char *input)
{
	char **tokens;
	int pos = 0;
	char *token;

	/*
	 * tokens array is going to be the same size as MAX_TOKENS
	 * change MAX_TOKENS above if you want it bigger
	 */
	tokens = malloc(MAX_TOKENS * sizeof(char *));

	if (!tokens)
	{
		perror("malloc");
		exit(1);
	}


	token = strtok(input, " \t");

	/*remember, 0 is the first entry*/
	while (token != NULL && pos < MAX_TOKENS - 1)
	{
		tokens[pos++] = token;
		token = strtok(NULL, " \t");
	}
	tokens[pos] = NULL;
	return (tokens);
}

/**
 * *read_input - reads the input from stdin
 * @stream: the stream of text
 * Return: returns the line back to be parsed
 */

char *read_input(FILE *stream)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = getline(&line, &len, stream); /*malloc is already done for us*/
	if (nread == -1)
	{
		free(line);
		return (NULL);
	}
	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0'; /*treat '\n' as the end*/
	return (line);
}

/**
 * * main - main function
 * @argc: argument counter
 * @argv: argument vector
 * @envp: environment parameters
 * Return: returns 0 on success, anything else is an error type
 */
int main(int argc, char **argv, char **envp)
{
	int running = 1, interactive; /*booleans*/
	char *input = NULL; /*line buffer*/
	char **tokens = NULL; /*token array*/
	int count = 0; /*Track command count*/

/*ignore unused parameter*/
(void)argc; 
(void)envp;

	interactive = isatty(STDIN_FILENO);
	while (running)
	{
		/*only happens when we directly type into terminal*/
		if (interactive)
		{
			printf("$ ");
			fflush(stdout);
		}
		/*Read one whole line of input*/
		input = read_input(stdin);
		if (!input)
		{
break; /*EOF or error*/
		}
			
			count++;
		/*Split line into tokens*/
		tokens = parse(input);
		/*Decide what we do with said tokens*/
		if (tokens && tokens[0])
{
    execute(tokens, argv[0], count);
}

		free(tokens);
		free(input);
	}
	return (0);
}
