#include "shell_v2.h"

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
* handle_vars - handles variable substitution
* @arg: arg to modify
* @db: reference to database struct
*/
char *handle_var(char *key, db_t *db)
{
	if (key[1] == '$')
		return (db->pid);
	if (key[1] != '\0')
		return (get_env(db, &key[1]));
	return (NULL);
}

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
