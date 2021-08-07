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
	int saved, fd, code;

	saved = dup(1);
	fd = open(cmd->right[0], (O_WRONLY | O_CREAT | O_TRUNC));
	dup2(fd, 1);

	code = execute_cmd(db, cmd->left);

	close(fd);

	dup2(saved, 1);
	close(saved);

	return (code);
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
	int saved, fd, code;

	saved = dup(1);
	fd = open(cmd->right[0], (O_WRONLY | O_CREAT | O_APPEND));
	dup2(fd, 1);

	code = execute_cmd(db, cmd->left);

	close(fd);

	dup2(saved, 1);
	close(saved);

	return (code);
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
	int saved, fd, code;

	saved = dup(0);
	fd = open(cmd->right[0], O_RDONLY);
	if (fd == -1)
		return (eprint(READ_ERR, db, cmd->right));

	dup2(fd, 0);

	code = execute_cmd(db, cmd->left);

	close(fd);

	dup2(saved, 0);
	close(saved);

	return (code);
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
