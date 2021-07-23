#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shell_v2.h"

/**
 * main - entry point for shell_v2
 *
 * Return: exits with provided code, else last error code, else 0
 */
int main(int ac, char **av, char **env)
{
	char *line = NULL;
	size_t len = 0;
	listcmd_t *list = NULL;
	int code = 0, interactive = isatty(0);

	(void)ac;
	(void)av;

	while (1)
	{
		if (interactive)
			printf("$ ");
		if (getline(&line, &len, stdin) == -1)
			break;
		list = build_cmds(line, env);
		execute_list(list);
		free_listcmd(list);
	}

	free(line);

	exit(code);
}
