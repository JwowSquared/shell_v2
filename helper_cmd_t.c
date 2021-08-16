#include "shell_v2.h"
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
* handle_var - handles variable substitution
* @key: key to match with environment variable
* @db: reference to database struct
*
* Return: pointer to string that replaces variable
*/
char *handle_var(char *key, db_t *db)
{
	if (key[1] == '$')
		return (db->pid);
	if (key[1] != '\0')
		return (get_env(db, &key[1]));
	return (key);
}

/**
* check_pstat - determines right before execution if $? is present
* @arg: arg to check
* @db: reference to database struct
*
* Return: -1 on malloc error, else 0
*/
int check_pstat(arg_t *arg, db_t *db)
{
	int i = 0;
	char *key = NULL;

	while (arg->av[i] != NULL)
	{
		key = arg->av[i];
		if (key[0] == '$' && key[1] == '?')
		{
			if (db->vstat != NULL)
				free(db->vstat);
			db->vstat = dup_atoi(db->pstat);
			if (db->vstat == NULL)
				return (-1);
			arg->av[i] = db->vstat;
		}
		i++;
	}
	return (0);
}
