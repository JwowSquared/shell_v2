#include "shell_v2.h"

/**
* op_semi - ; separator
* @list: reference to listcmd struct
* @cmd: current cmd struct to execute
*
* Return: always 0
*/
int op_semi(listcmd_t *list, cmd_t *cmd)
{
	(void)cmd;
	(void)list;

	return (0);
}

/**
* op_or - || separator
* @list: reference to listcmd struct
* @cmd: current cmd struct to execute
*
* Return: 1 if pstat is 0, else 0
*/
int op_or(listcmd_t *list, cmd_t *cmd)
{
	(void)cmd;

	if (list->pstat == 0)
		return (1);
	return (0);
}

/**
* op_and - && separator
* @list: reference to listcmd struct
* @cmd: current cmd struct to execute
*
* Return: 0 if pstat is 0, else 1
*/
int op_and(listcmd_t *list, cmd_t *cmd)
{
	(void)cmd;

	if (list->pstat == 0)
		return (0);
	return (1);
}
