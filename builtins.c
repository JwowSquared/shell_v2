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
	(void)db;
	(void)cmd;

	printf("BUILTIN ENV\n");

	return (1);
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
	(void)db;
	(void)cmd;

	printf("BUILTIN SETENV\n");

	return (1);
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
	(void)db;
	(void)cmd;

	printf("BUILTIN UNSETENV\n");

	return (1);
}
