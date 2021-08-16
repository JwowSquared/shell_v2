#include "shell_v2.h"

/**
* handle_comments - handles #
* @arg: arg to modify
*/
void handle_comments(arg_t *arg)
{
	int i = 0;

	while (arg->av[i] != NULL)
	{
		if (arg->av[i][0] == '#')
		{
			arg->av[i] = NULL;
			break;
		}
		i++;
	}
}

/**
* handle_vars - handles variable substitution
* @arg: arg to modify
* @db: reference to database struct
*/
int handle_vars(arg_t *arg, db_t *db)
{
	int i = 0;
	char *key = NULL;

	while (arg->av[i] != NULL)
	{
		key = arg->av[i];
		if (key[0] == '$')
		{
			if (key[1] == '\0')
			{
				i++;
				continue;
			}
			if (key[1] == '$')
				arg->av[i] = db->pid;
			else if (key[1] == '?')
			{
				if (db->vstat != NULL)
					free(db->vstat);
				db->vstat = dup_atoi(db->pstat);
				if (db->vstat == NULL)
					return (-1);
				arg->av[i] = db->vstat;
			}
			else
				arg->av[i] = get_env(db, &key[1]);
			if (i == 0)
			{
				if (arg->path != NULL)
					free(arg->path);
				arg->path = NULL;
				arg->check_path = setup_path(arg, db);
				if (arg->check_path == -2 || errno == ENOMEM)
					return (-1);
				if (arg->path == NULL && arg->check_path == 0)
				{
					arg->check_path = 1;
					arg->path = arg->av[0];
				}
			}
		}
		i++;
	}
	return (0);
}
