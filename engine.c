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
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	cmd_t *cmd = NULL;
	int code = 0, interactive = isatty(0);

	while (1)
	{
		if (interactive)
			printf("$ ");
		if (getline(&line, &len, stdin) == -1)
			break;
		cmd = build_cmd(line);
		if (cmd->opf != NULL)
			cmd->opf(cmd);
		else
			printf("execute command normally.\n");
		free_cmd(cmd);
	}

	free(line);

	exit(code);
}

int example(cmd_t *cmd)
{
	int i;

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: |%s|\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: |%s|\n", i, cmd->right[i]);

	printf("For use with |%s|\n", cmd->op);

	return (1);
}
