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
	env_t *current;
	char *key = cmd[0], *val = cmd[1];

	current = db->envh;
	while (current != NULL)
	{
		for (i = 0; key[i]; i++)
			if (key[i] != current->s[i])
				break;
		if (key[i] == '\0' && current->s[i] == '=')
			break;
		current = current->next;
	}

	if (current == NULL)
	{
		current = malloc(sizeof(env_t));
		if (current == NULL)
			return (NULL);
		current->next = db->envh;
	}
	else
		free(current->s);

	current->s = malloc(sizeof(char) * (i + _strlen(val) + 2));
	if (current->s == NULL)
	{
		free(current);
		return (NULL);
	}
	db->envh = current;

	if (cmd[2] != NULL)
		sprintf(current->s, "%s=%s", key, val);
	else
		sprintf(current->s, "%s=", key);

	return (current);
}
