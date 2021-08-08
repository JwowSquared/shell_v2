#include "shell_v2.h"

/**
* build_cmd - converts a command line into a cmd struct
* @line: command line to convert
* @psep: the previous separate operator
*
* Return: pointer to new struct, else NULL
*/
cmd_t *build_cmd(char *line, char psep)
{
	cmd_t *out;
	int i = 0, len;
	char c;

	out = malloc(sizeof(cmd_t));
	if (out == NULL)
		return (NULL);
	out->opf = NULL;
	out->psep = sball(psep);
	len = _strlen(line);
	for (i = 0; i < len; i++)
	{
		out->opf = rball(&line[i]);
		if (out->opf != NULL)
		{
			c = line[i];
			line[i++] = '\0';
			if (c == line[i])
				i++;
			break;
		}
	}
	out->left = malloc(sizeof(char *) * (count_words(line) + 1));
	if (out->left == NULL)
		return (free_cmd(out));
	out->right = malloc(sizeof(char *) * (count_words(&line[i]) + 1));
	if (out->right == NULL)
		return (free_cmd(out));

	cut_line(out, line, &line[i]);

	return (out);
}

/**
* execute_cmd - executes a command
* @db: reference to database struct
* @cmd: double char pointer containing the command to execute
*
* Return: status code of command that was executed
*/
int execute_cmd(db_t *db, char **cmd)
{
	int status, path_flag = 0;
	int (*bi)(db_t *, char **);
	char *path = NULL;

	bi = bball(cmd[0]);
	if (bi != NULL)
		return (bi(db, cmd));

	db->env = format_env(db);
	if (db->env == NULL)
		return (eprint(MALLOC_ERR, db, cmd));

	if (cmd[0] == NULL)
		return (0);

	status = check_path(db, cmd[0], &path);
	if (status == -1 && errno != ENOMEM)
		return (eprint(PATH_ERR, db, cmd));
	if (status == -2 || errno == ENOMEM)
		return (eprint(MALLOC_ERR, db, cmd));

	if (path == NULL)
	{
		path = cmd[0];
		path_flag = 1;
	}

	if (!fork())
	{
		execve(path, cmd, db->env);
		perror(NULL);
		exit(2);
	}
	wait(&status);

	if (path_flag == 0)
		free(path);

	return (WEXITSTATUS(status));
}

/**
* free_cmd - frees the cmd_t struct
* @cmd: pointer to struct to free
*
* Return: always NULL
*/
void *free_cmd(cmd_t *cmd)
{
	if (cmd == NULL)
		return (NULL);

	if (cmd->left != NULL)
		free(cmd->left);
	if (cmd->right != NULL)
		free(cmd->right);

	free(cmd);

	return (NULL);
}
