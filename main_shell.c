#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell_components.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STRTOK_DELIM " \t\n\a\r"
#define STRTOK_BUFSIZE 64

/**
 * line_reader - this function reads the line from stdout
 *
 * Return: returns the line.
 */
char *line_reader(void)
{
	char *line = NULL;
	ssize_t len = 0;

	if (getline(&line, &len, stdin) == -1)
	{
		if (feof(stdin))
		{
			exit(EXIT_SUCCESS);
		}
		else
		{
			perror("readline");
			exit(EXIT_FAILURE);
		}
	}
	return (line);
}

/**
 * line_splitter - splits the line of text inputed
 * @line: the line from stdin
 *
 * Return: an array of string pointers.
 */
char **line_parser(char *line)
{
	int bufsize = STRTOK_BUFSIZE, position = 0;
	char **parsed_line = malloc(bufsize * sizeof(char *));
	char *token;

	if (!parsed_line)
	{
		fprintf(stderr, "allocation failure\n");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, STRTOK_DELIM);
	while (token != NULL)
	{
		parsed_line[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize = bufsize + STRTOK_BUFSIZE;
			parsed_line = realloc(parsed_line, bufsize * sizeof(char *));
			if (!parsed_line)
			{
				fprintf(stderr, "allocation failure\n");
				exit(EXIT_FAILURE);
			}
		}
	token = strtok(NULL, STRTOK_DELIM);
	}
	parsed_line[position] = NULL;
	return (parsed_line);
}

/**
 * executioner - it executes a command from the terminal
 * @parsed_line: an array of pointer strings containing the
 * commands.
 *
 * Return: the execution of the process.
 */
int executioner(char **parsed_line)
{
	pid_t exe_process;
	char *env[] = {NULL};
	int status;

	exe_process = fork();
	if (exe_process == -1)
	{
		perror("Error:");
		return (-1);
	}
	if (exe_process == 0)
	{
		if (execve(parsed_line[0], parsed_line, env) == -1)
			perror("Error:");
	}
	else
	{
		wait(&status);
	}
	return (1);
}

/**
 * shell_loop - contains the loop for the shell
 *
 * Return: returns nothing
 */
void main(void)
{
	char *line;
	char **commands;
	int status;

	do {
		printf("#fountain-shell$ ");
		line = line_reader();
		commands = line_parser(line);
		status = executioner(commands);

		free(line);
		free(commands);
	} while (status);
}
