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
* build2_cmd - converts a command line into a cmd struct
* @line: command line to convert
* @psep: the previous separate operator
*
* Return: pointer to new struct, else NULL
*/
cmd_t *build2_cmd(char *line, char psep)
{
	cmd_t *out;
	int i = 0, len;
	char c;

	out = malloc(sizeof(cmd_t));
	if (out == NULL)
		return (NULL);
	out->opf = NULL;
	out->psep = sball2(psep);
	len = _strlen(line);
	for (i = 0; i < len; i++)
	{
		out->opf = rball2(&line[i]);
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
* cut_line - strtoks left and right into out
* @out: command struct being built
* @left: line to cut into left
* @right: line to cut into right
*/
void cut_line(cmd_t *out, char *left, char *right)
{
	int i = 0;
	char *tmp;

	for (i = 0; (tmp = strtok(left, " ")); i++)
	{
		left = NULL;
		out->left[i] = tmp;
	}
	out->left[i] = NULL;
	for (i = 0; (tmp = strtok(right, " ")); i++)
	{
		right = NULL;
		out->right[i] = tmp;
	}
	out->right[i] = NULL;
}

int count_words(char *line)
{
	int i = 0, out = 0;

	if (line == NULL || line[0] == '\0')
		return (0);

	while (line[i] == ' ')
		i++;
	while (line[i])
	{
		out++;
		while (line[i] && line[i] != ' ')
			i++;
		while (line[i] && line[i] == ' ')
			i++;
	}
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

	status = check_path(db, cmd);
	if (status == -1 && errno != ENOMEM)
		return (eprint(PATH_ERR, db, cmd));
	if (status == -2 || errno == ENOMEM)
		return (eprint(MALLOC_ERR, db, cmd));

	if (!fork())
	{
		execve(cmd[0], cmd, db->env);
		perror(NULL);
		exit(2);
	}
	wait(&status);

	if (db->p_diff)
		free(cmd[0]);
	db->p_diff = 0;
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
