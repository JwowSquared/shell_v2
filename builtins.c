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
	int result = 0, i = 0, tmp;

	if (cmd[1] == NULL)
	{
		db->toexit = 1;
		return (db->pstat);
	}

	while (cmd[1][i] != '\0')
	{
		tmp = cmd[1][i] - '0';
		if (tmp < 0 || tmp > 9)
			return (eprint(EXIT_ERR, db, cmd));
		result *= 10;
		result += tmp;
		i++;
	}

	db->toexit = 1;
	return (result);
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
	char *tmp = NULL, *dest = cmd[1];

	if (dest == NULL)
		dest = get_env(db, "HOME");
	else if (!_strcmp(dest, "-"))
		dest = get_env(db, "OLDPWD");

	if (dest == NULL)
	{
		if (_strcmp(cmd[1], "-"))
			printf("%s\n", get_env(db, "PWD"));
		return (0);
	}

	if (chdir(dest))
		return (eprint(CD_ERR, db, cmd));

	if (cmd[1] && !_strcmp(cmd[1], "-"))
	{
		tmp = _strdup(dest);
		if (tmp == NULL)
			return (eprint(MALLOC_ERR, db, cmd));
	}
	else
		tmp = dest;

	if (!insert_env(db, "OLDPWD", get_env(db, "PWD")))
		return (eprint(MALLOC_ERR, db, cmd));
	if (!insert_env(db, "PWD", tmp))
		return (eprint(MALLOC_ERR, db, cmd));

	if (tmp != dest)
	{
		printf("%s\n", tmp);
		free(tmp);
	}

	return (0);
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
	int i = 0;
	env_t *current;

	if (cmd[1] == NULL)
		return (eprint(SETENV_ERR1, db, cmd));

	while (cmd[1][i] != '\0')
		if (cmd[1][i++] == '=')
			return (eprint(SETENV_ERR2, db, cmd));

	current = insert_env(db, cmd[1], cmd[2]);
	if (current == NULL)
		return (eprint(MALLOC_ERR, db, cmd));

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
		return (eprint(UNSET_ERR1, db, cmd));

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

	return (eprint(UNSET_ERR2, db, cmd));
}
