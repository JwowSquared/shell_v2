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
		exit(eprint(DB_ERR, NULL, av));

	while (!db->toexit)
	{
		if (interactive)
			printf("$ ");
		if (getline(&line, &len, stdin) == -1)
			break;
		list = build_cmds(line, db);
		if (list == NULL)
		{
			db->pstat = eprint(MALLOC_ERR, db, NULL);
			break;
		}
		execute_list(list);
		free_listcmd(list);
		db->ln++;
	}

	free(line);
	code = db->pstat;
	free_db(db);
	exit(code);
}

/**
* check_path - determines if the command exists in the path
* @db: reference to database struct
* @cmd: current command being executed
*
* Return: 0 on success, -1 when not found, -2 on malloc error
*/
int check_path(db_t *db, char **cmd)
{
	struct stat st;
	int i = 0, match = 0;
	char *slice, *out, *p, *name = cmd[0], *path = get_env(db, "PATH");

	while (name[i])
		if (name[i++] == '/')
			match = 1;

	if (match || path == NULL)
	{
		i = lstat(name, &st);
		if (i == -1 && errno == ENOMEM)
			return (-2);
		return (i);
	}

	path = _strdup(path);
	if (path == NULL)
		return (-2);
	p = path;
	while ((slice = strtok(p, ":")))
	{
		p = NULL;
		out = malloc(sizeof(char) * (2 + _strlen(name) + _strlen(slice)));
		if (out == NULL)
		{
			free(path);
			return (-2);
		}
		sprintf(out, "%s/%s", slice, name);
		match = lstat(out, &st);
		if (match == 0)
			break;
		free(out);
		out = NULL;
		if (errno == ENOMEM)
			break;
	}

	free(path);

	if (errno == ENOMEM)
		return (-2);
	if (out == NULL)
		return (-1);
	cmd[0] = out;
	db->p_diff = 1;
	return (0);
}
