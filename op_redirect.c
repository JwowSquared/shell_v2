#include "shell_v2.h"

/**
* op_write - > redirect
* @db: reference to database struct
* @cmd: current cmd struct to execute
*
* Return: 0 on success, else error code is returned
*/
int op_write(db_t *db, cmd_t *cmd)
{
	int i;

	(void)db;

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->right[i]);

	printf("For use with [>]\n");

	return (1);
}

/**
* op_append - >> redirect
* @db: reference to database struct
* @cmd: current cmd struct to execute
*
* Return: 0 on success, else error code is returned
*/
int op_append(db_t *db, cmd_t *cmd)
{
	int i;

	(void)db;

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->right[i]);

	printf("For use with [>>]\n");

	return (1);
}

/**
* op_read - < redirect
* @db: reference to database struct
* @cmd: current cmd struct to execute
*
* Return: 0 on success, else error code is returned
*/
int op_read(db_t *db, cmd_t *cmd)
{
	int i;

	(void)db;

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->right[i]);

	printf("For use with [<]\n");

	return (1);
}

/**
* op_heredoc - << redirect
* @db: reference to database struct
* @cmd: current cmd struct to execute
*
* Return: 0 on success, else error code is returned
*/
int op_heredoc(db_t *db, cmd_t *cmd)
{
	int i;

	(void)db;

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->right[i]);

	printf("For use with [<<]\n");

	return (1);
}

/**
* op_pipe - | redirect
* @db: reference to database struct
* @cmd: current cmd struct to execute
*
* Return: 0 on success, else error code is returned
*/
int op_pipe(db_t *db, cmd_t *cmd)
{
	int i;

	(void)db;

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: [%s]\n", i, cmd->right[i]);

	printf("For use with [|]\n");

	return (1);
}
