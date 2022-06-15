#ifndef SHELL_COMPONENTS_H
#define SHELL_COMPONENTS_H

#include <stddef.h>
#include <stdio.h>

char *line_reader(void);
int executioner(char **parsed_line);
char **line_parser(char *line);

#endif

