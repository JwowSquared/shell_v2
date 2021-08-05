#include "shell_v2.h"

/**
* insert_env - creates a new node if needed, else updates old match
* @db: reference to database struct
* @cmd: current command being executed
*
* Return: pointer to new node, else NULL
*/
env_t *insert_env(db_t *db, char **cmd)
{
	int i = 0;
	env_t *current, *prev = NULL;
	char *cat, *key = cmd[1], *val = cmd[2];

	cat = malloc(sizeof(char) * (_strlen(key) + _strlen(val) + 2));
	if (cat == NULL)
		return (NULL);

	current = db->envh;
	while (current != NULL)
	{
		for (i = 0; key[i]; i++)
			if (key[i] != current->s[i])
				break;
		if (key[i] == '\0' && current->s[i] == '=')
			break;
		prev = current;
		current = current->next;
	}

	if (current == NULL)
	{
		current = malloc(sizeof(env_t));
		if (current == NULL)
			return (NULL);
		current->next = NULL;
		if (prev != NULL)
			prev->next = current;
		else
			db->envh = current;
	}
	else
		free(current->s);
	current->s = cat;
	if (cmd[2] != NULL)
		sprintf(current->s, "%s=%s", key, val);
	else
		sprintf(current->s, "%s=", key);

	return (current);
}
