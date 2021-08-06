#include "shell_v2.h"

/**
* insert_env - creates a new node if needed, else updates old match
* @db: reference to database struct
* @key: key of env to update or add
* @val: value to be associated with key
* Return: pointer to new node, else NULL
*/
env_t *insert_env(db_t *db, char *key, char *val)
{
	int i = 0;
	env_t *current, *prev = NULL;
	char *cat;

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
	if (val != NULL)
		sprintf(current->s, "%s=%s", key, val);
	else
		sprintf(current->s, "%s=", key);

	return (current);
}

/**
* get_env - retrieves the value of an environment variable
* @db: reference to database struct
* @key: key to match with value
*
* Return: pointer to value, else NULL
*/
char *get_env(db_t *db, char *key)
{
	int i;
	env_t *current;

	if (key == NULL)
		return (NULL);

	current = db->envh;
	while (current != NULL)
	{
		for (i = 0; key[i] != '\0'; i++)
			if (current->s[i] != key[i])
				break;
		if (key[i] == '\0' && current->s[i] == '=')
			break;
		current = current->next;
	}

	if (current == NULL)
		return (NULL);

	return (&(current->s[i + 1]));
}
