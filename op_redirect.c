#include "shell_v2.h"

/**
* op_write - > redirect
* @list: reference to listcmd struct
* @cmd: current cmd struct to execute
*
* Return: 0 on success, else error code is returned
*/
int op_write(listcmd_t *list, cmd_t *cmd)
{
	int i;

	(void)list;

	if (cmd->psep != NULL)
		printf("SEPERATOR = [%s]\n", cmd->psep);

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->right[i]);

	printf("For use with [>]\n");

	return (1);
}

/**
* op_append - >> redirect
* @list: reference to listcmd struct
* @cmd: current cmd struct to execute
*
* Return: 0 on success, else error code is returned
*/
int op_append(listcmd_t *list, cmd_t *cmd)
{
	int i;

	(void)list;

	if (cmd->psep != NULL)
		printf("SEPERATOR = [%s]\n", cmd->psep);

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->right[i]);

	printf("For use with [>>]\n");

	return (1);
}

/**
* op_read - < redirect
* @list: reference to listcmd struct
* @cmd: current cmd struct to execute
*
* Return: 0 on success, else error code is returned
*/
int op_read(listcmd_t *list, cmd_t *cmd)
{
	int i;

	(void)list;

	if (cmd->psep != NULL)
		printf("SEPERATOR = [%s]\n", cmd->psep);

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->right[i]);

	printf("For use with [<]\n");

	return (1);
}

/**
* op_heredoc - << redirect
* @list: reference to listcmd struct
* @cmd: current cmd struct to execute
*
* Return: 0 on success, else error code is returned
*/
int op_heredoc(listcmd_t *list, cmd_t *cmd)
{
	int i;

	(void)list;

	if (cmd->psep != NULL)
		printf("SEPERATOR = [%s]\n", cmd->psep);

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->right[i]);

	printf("For use with [<<]\n");

	return (1);
}

/**
* op_pipe - | redirect
* @list: reference to listcmd struct
* @cmd: current cmd struct to execute
*
* Return: 0 on success, else error code is returned
*/
int op_pipe(listcmd_t *list, cmd_t *cmd)
{
	int i;

	(void)list;

	if (cmd->psep != NULL)
		printf("SEPERATOR = [%s]\n", cmd->psep);

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->right[i]);

	printf("For use with [|]\n");

	return (1);
}
