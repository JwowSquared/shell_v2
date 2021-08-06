#include "shell_v2.h"

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
	char *slice, *out = NULL, *p, *name = cmd[0], *path = get_env(db, "PATH");

	while (name[i])
		if (name[i++] == '/')
			match = 1;
	if (match)
		return (lstat(name, &st));
	if (path == NULL)
		return (-1);
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
	if (out == NULL)
		return (-1);
	cmd[0] = out;
	db->p_diff = 1;
	return (0);
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

/**
* count_words - counts the words in a string
* @line: string with words to count
*
* Return: number of words in line, else 0
*/
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
