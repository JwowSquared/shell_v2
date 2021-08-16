#include "shell_v2.h"

/**
* free_arg - frees the memory associated with the arg_t struct
* @arg: arg to free
*
* Return: Always NULL
*/
void *free_arg(arg_t *arg)
{
	if (arg->av != NULL)
		free(arg->av);
	if (arg->path != NULL && arg->check_path != 1)
		free(arg->path);
	free(arg);

	return (NULL);
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
