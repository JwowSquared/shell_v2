#include "shell_v2.h"

/**
* execute_pipe - used with the | redirect operator
* @db: reference to database struct
* @cmd: current cmd being executed
* @left: path of left command for use with execve
* @right: path of right command for use with execve
*
* Return: 0 on success, else proper error code
*/
int execute_pipe(db_t *db, cmd_t *cmd, char *left, char *right)
{
	int status, saved_out, saved_in;
	int pipefd[2];

	saved_out = dup(1);
	saved_in = dup(0);
	if (!fork())
	{
		pipe(pipefd);
		if (!fork())
		{
			close(pipefd[0]);
			dup2(pipefd[1], 1);
			execve(left, cmd->left, db->env); /* 1st command */
			perror(NULL);
			_exit(2);
		}
		wait(&status);
		dup2(saved_out, 1);
		close(saved_out);
		if (WEXITSTATUS(status) == 2)
			_exit(2);
		close(pipefd[1]);
		dup2(pipefd[0], 0);
		execve(right, cmd->right, db->env); /* 2nd command */
		perror(NULL);
		_exit(2);
	}
	wait(&status);
	dup2(saved_in, 0);
	close(saved_in);
	return (0);
}

/**
* check_path - determines if the command exists in the path
* @db: reference to database struct
* @name: current path to be checked
* @buf: buffer for new string to be stored in
*
* Return: 0 on success, -1 when not found, -2 on malloc error
*/
int check_path(db_t *db, char *name, char **buf)
{
	struct stat st;
	int i = 0, match = 0;
	char *slice, *out = NULL, *p, *path = get_env(db, "PATH");

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
	*buf = out;

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
