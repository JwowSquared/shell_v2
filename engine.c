#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shell_v2.h"

/**
* main - entry point for shell_v2
* @ac: unused
* @av: unused
* @env: used to grab current environment variables for use with execve
*
* Return: exits with provided code, else last error code, else 0
*/
int main(int ac, char **av, char **env)
{
	char *line = NULL;
	size_t len = 0;
	listcmd_t *list = NULL;
	db_t *db = NULL;
	int code = 0, interactive = isatty(0);

	(void)ac;

	db = build_db(av[0], env);
	if (db == NULL)
		return (1); /* What to do on malloc fail? */

	while (!db->toexit)
	{
		if (interactive)
			printf("$ ");
		if (getline(&line, &len, stdin) == -1)
			break;
		list = build_cmds(line, db);
		execute_list(list);
		free_listcmd(list);
		db->ln++;
	}

	free(line);
	code = db->pstat;
	free_db(db);
	exit(code);
}
