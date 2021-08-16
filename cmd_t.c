#include "shell_v2.h"

/**
* build_cmd - converts a command line into a cmd struct
* @db: reference to database struct
* @line: command line to convert
* @psep: the previous separate operator
*
* Return: pointer to new struct, else NULL
*/
cmd_t *build_cmd(db_t *db, char *line, char psep)
{
	cmd_t *out;
	arg_t *prev = NULL, *current;
	int i = 0, s, len;
	char c;
	int (*opf)(db_t *, arg_t *);

	out = malloc(sizeof(cmd_t));
	if (out == NULL)
		return (NULL);
	out->head = NULL;
	out->opf = NULL;
	out->psep = sball(psep);
	len = _strlen(line);
	while (i < len)
	{
		for (s = i; i < len; i++)
		{
			opf = rball(&line[i]);
			if (opf != NULL)
			{
				out->opf = opf;
				c = line[i];
				line[i] = '\0';
				if (c == line[i + 1])
					i++;
				break;
			}
		}
		current = build_arg(&line[s], db);
		if (current == NULL)
			return (free_cmd(out));
		if (prev != NULL)
			prev->next = current;
		else
			out->head = current;
		prev = current;
		i++;
	}

	return (out);
}


/**
* build_arg - builds an arg_t linked list node
* @line: line to split into an array of tokens
* @db: reference to database struct
*
* Return: pointer to new struct, else NULL
*/
arg_t *build_arg(char *line, db_t *db)
{
	arg_t *out;
	char *tmp;
	int i;

	out = malloc(sizeof(arg_t));
	if (out == NULL)
		return (NULL);

	out->next = NULL;
	out->path = NULL;
	out->check_path = 0;
	out->av = malloc(sizeof(char *) * (count_words(line) + 1));
	if (out->av == NULL)
	{
		free(out);
		return (NULL);
	}

	for (i = 0; (tmp = strtok(line, " ")); i++)
	{
		line = NULL;
		out->av[i] = tmp;
	}
	out->av[i] = NULL;

	out->check_path = setup_path(out, db);
	if (out->check_path == -2 || errno == ENOMEM)
	{
		free(out->av);
		free(out);
		return (NULL);
	}

	if (out->path == NULL && out->check_path == 0)
	{
		out->check_path = 1;
		out->path = out->av[0];
	}
	handle_comments(out);
	return (out);
}

/**
* setup_path - determines if their is a valid path for arg, and sets check_path
* @arg: arg struct to determine file name and store path
* @db: reference to database struct, used for get_env
*
* Return: 0 on sucess, -1 on path fail or lstat malloc fail, -2 on malloc fail
*/
int setup_path(arg_t *arg, db_t *db)
{
	struct stat st;
	int i = 0;
	char *slice, *out = NULL, *p, *name = arg->av[0], *path = get_env(db, "PATH");

	if (name == NULL)
		return (0);

	while (name[i])
		if (name[i++] == '/')
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
		if (lstat(out, &st) == 0)
			break;
		free(out);
		out = NULL;
		if (errno == ENOMEM)
			break;
	}
	free(path);
	if (out == NULL)
		return (-1);
	arg->path = out;

	return (0);
}

/**
* execute_arg - executes a command
* @db: reference to database struct
* @arg: current arg struct being executed
*
* Return: status code of command that was executed
*/
int execute_arg(db_t *db, arg_t *arg)
{
	int status;
	int (*bi)(db_t *, char **);

	if (handle_vars(arg, db) == -1)
		return (eprint(MALLOC_ERR, db, NULL));

	bi = bball(arg->av[0]);
	if (bi != NULL)
		return (bi(db, arg->av));

	db->env = format_env(db);
	if (db->env == NULL)
		return (eprint(MALLOC_ERR, db, NULL));

	if (arg->av[0] == NULL)
		return (0);

	if (arg->check_path == -1)
		return (eprint(PATH_ERR, db, arg->av));

	if (!fork())
	{
		execve(arg->path, arg->av, db->env);
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
	arg_t *tmp;

	if (cmd == NULL)
		return (NULL);

	tmp = cmd->head;
	while (tmp != NULL)
	{
		cmd->head = cmd->head->next;
		free(tmp->av);
		if (tmp->path && tmp->check_path != 1)
			free(tmp->path);
		free(tmp);
		tmp = cmd->head;
	}

	free(cmd);

	return (NULL);
}
