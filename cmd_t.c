#include "shell_v2.h"

/**
* build_cmd - converts a command line into a cmd struct
* @line: command line to convert
* @psep: the previous separate operator
*
* Return: pointer to new struct, else NULL
*/
cmd_t *build_cmd(char *line, char *psep)
{
	cmd_t *out;
	char *temp, *word;
	int w_left = 1, w_right = 0, i;

	out = malloc(sizeof(cmd_t));
	if (out == NULL)
		return (NULL);
	out->opf = NULL;
	out->psep = sball(psep);
	line = strtok(line, "\n");
	temp = _strdup(line);
	word = strtok(temp, " ");
	while ((word = strtok(NULL, " ")))
	{
		out->opf = rball(word);
		if (out->opf != NULL)
			break;
		w_left++;
	}
	while ((word = strtok(NULL, " ")))
		w_right++;
	free(temp);

	out->left = malloc(sizeof(char *) * (w_left + 1));
	if (out->left == NULL)
		return (free_cmd(out));
	out->right = malloc(sizeof(char *) * (w_right + 1));
	if (out->right == NULL)
		return (free_cmd(out));

	out->left[0] = strtok(line, " ");
	for (i = 1; i < w_left; i++)
		out->left[i] = strtok(NULL, " ");
	out->left[i] = NULL;
	strtok(NULL, " "); /* skips over Operator */
	for (i = 0; i < w_right; i++)
		out->right[i] = strtok(NULL, " ");
	out->right[i] = NULL;

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
	int status;
	int (*bi)(db_t *, char **);

	bi = bball(cmd[0]);
	if (bi != NULL)
		return (bi(db, cmd));

	db->env = format_env(db);
	if (db->env == NULL)
		return (eprint(MALLOC_ERR, db, cmd));

	if (!fork())
	{
		execve(cmd[0], cmd, db->env);
		perror(NULL);
		exit(2);
	}
	wait(&status);
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
