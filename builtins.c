#include "shell_v2.h"

/**
* bi_exit - exit builtin
* @db: reference to database struct
* @cmd: tokenized command to execute
*
* Return: 0 on success, else error code is returned
*/
int bi_exit(db_t *db, char **cmd)
{
	db->toexit = 1;

	if (cmd[1] != NULL)
		return (_atoi(cmd[1], db));

	return (db->pstat);
}

/**
* bi_cd - cd builtin
* @db: reference to database struct
* @cmd: tokenized command to execute
*
* Return: 0 on success, else error code is returned
*/
int bi_cd(db_t *db, char **cmd)
{
	(void)db;
	(void)cmd;

	printf("BUILTIN CD\n");

	return (1);
}

/**
* bi_env - env builtin
* @db: reference to database struct
* @cmd: tokenized command to execute
*
* Return: 0 on success, else error code is returned
*/
int bi_env(db_t *db, char **cmd)
{
	env_t *current;

	(void)cmd;

	current = db->envh;
	while (current != NULL)
	{
		printf("%s\n", current->s);
		current = current->next;
	}

	return (0);
}

/**
* bi_setenv - setenv builtin
* @db: reference to database struct
* @cmd: tokenized command to execute
*
* Return: 0 on success, else error code is returned
*/
int bi_setenv(db_t *db, char **cmd)
{
	int i = 0, j = 0;
	env_t *current;

	if (cmd[1] == NULL)
	{
		perror("i think this is supposed to call env?\n");
		return (2);
	}

	while (cmd[1][i] != '\0')
		if (cmd[1][i++] == '=')
		{
			perror("Key cant have equals sign\n");
			return (2);
		}

	if (cmd[2] != NULL)
		while (cmd[2][j] != '\0')
			j++;

	current = db->envh;
	while (current != NULL)
	{
		if (_strcmp(current->s, cmd[1]) == '=')
			break;
		current = current->next;
	}

	if (current == NULL)
	{
		current = malloc(sizeof(env_t));
		if (current == NULL)
			return (2); /* What do on malloc fail? */
		current->next = db->envh;
		db->envh = current;
	}
	else
		free(current->s);

	current->s = malloc(sizeof(char) * (i + j + 2));
	if (current->s == NULL)
	{
		db->envh = current->next;
		free(current);
		return (2); /* What do on malloc fail? */
	}

	if (cmd[2] != NULL)
		sprintf(current->s, "%s=%s", cmd[1], cmd[2]);
	else
		sprintf(current->s, "%s=", cmd[1]);


	db->h_diff = 1;
	db->h_size++;

	return (0);
}

/**
* bi_unsetenv - unsetenv builtin
* @db: reference to database struct
* @cmd: tokenized command to execute
*
* Return: 0 on success, else error code is returned
*/
int bi_unsetenv(db_t *db, char **cmd)
{
	env_t *current, *prev = NULL;

	if (cmd[1] == NULL)
		return (2); /* what do here? */

	current = db->envh;
	while (current != NULL)
	{
		if (_strcmp(current->s, cmd[1]) == '=')
		{
			if (prev != NULL)
				prev->next = current->next;
			else
				db->envh = current->next;
			free(current->s);
			free(current);
			db->h_size--;
			db->h_diff = 1;
			return (0);
		}
		prev = current;
		current = current->next;
	}

	return (2); /* didnt find it! */
}
