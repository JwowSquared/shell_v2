#include "shell_v2.h"

/**
* op_semi - ; separator
* @pstat: status code from the last executed command
*
* Return: always 0
*/
int op_semi(int pstat)
{
	(void)pstat;

	return (0);
}

/**
* op_or - || separator
* @pstat: status code from the last executed command
*
* Return: 1 if pstat is 0, else 0
*/
int op_or(int pstat)
{
	if (pstat == 0)
		return (1);
	return (0);
}

/**
* op_and - && separator
* @pstat: status code from the last executed command
*
* Return: 0 if pstat is 0, else 1
*/
int op_and(int pstat)
{
	if (pstat == 0)
		return (0);
	return (1);
}
