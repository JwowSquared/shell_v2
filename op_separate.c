#include "shell_v2.h"

/**
* op_semi - ; separator
* @db: reference to database struct
* @cmd: current cmd struct to execute
*
* Return: always 0
*/
int op_semi(db_t *db, cmd_t *cmd)
{
	(void)cmd;
	(void)db;

	return (0);
}

/**
* op_or - || separator
* @db: reference to database struct
* @cmd: current cmd struct to execute
*
* Return: 1 if pstat is 0, else 0
*/
int op_or(db_t *db, cmd_t *cmd)
{
	(void)cmd;

	if (db->pstat == 0)
		return (1);
	return (0);
}

/**
* op_and - && separator
* @db: reference to database struct
* @cmd: current cmd struct to execute
*
* Return: 0 if pstat is 0, else 1
*/
int op_and(db_t *db, cmd_t *cmd)
{
	(void)cmd;

	if (db->pstat == 0)
		return (0);
	return (1);
}
