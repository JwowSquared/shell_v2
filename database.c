#include "shell_v2.h"

/**
* build_db - creates a database struct
* @pname: program name
* @env: given immutable environment variables
*
* Return: pointer to new struct, else NULL
*/
db_t *build_db(char *pname, char **env)
{
	int i = 0;
	db_t *db;
	env_t *tmp;

	db = malloc(sizeof(db_t));
	if (db == NULL)
		return (NULL);

	db->pname = pname;
	db->ln = 1;
	db->toexit = 0;
	db->pstat = 0;
	db->envh = NULL;
	while (env[i] != NULL)
	{
		tmp = malloc(sizeof(env_t));
		if (tmp == NULL)
			return (free_db(db));

		tmp->s = _strdup(env[i]);
		if (tmp->s == NULL)
			return (free_db(db));

		tmp->next = db->envh;
		db->envh = tmp;
		i++;
	}
	db->h_size = i + 1;
	db->a_max = 0;
	db->env = NULL;
	db->h_diff = 1;

	return (db);
}

/**
* format_env - updates db->env if any changes have been made to db->envh
* @db: database struct holding relevant pointers
*
* Return: pointer to new or old env array, else NULL
*/
char **format_env(db_t *db)
{
	int i = 0;
	char **out;
	env_t *current;

	if (!db->h_diff)
		return (db->env);

	if (db->h_size > db->a_max)
	{
		if (db->env != NULL)
			free(db->env);
		db->a_max = db->h_size;
		out = malloc(sizeof(char *) * (db->a_max + 1));
		if (out == NULL)
			return (NULL);
	}
	else
		out = db->env;

	current = db->envh;
	while (current != NULL)
	{
		out[i++] = current->s;
		current = current->next;
	}
	out[i] = NULL;

	db->h_diff = 0;
	return (out);
}

/**
* free_db - frees a database struct
* @db: pointer to struct to free
*
* Return: always NULL
*/
void *free_db(db_t *db)
{
	env_t *current;

	if (db == NULL)
		return (NULL);

	current = db->envh;
	while (current != NULL)
	{
		if (current->s != NULL)
			free(current->s);
		db->envh = db->envh->next;
		free(current);
		current = db->envh;
	}

	if (db->env != NULL)
		free(db->env);

	free(db);

	return (NULL);
}
