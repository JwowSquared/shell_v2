#include "shell_v2.h"

/**
* op_write - > redirect
* @db: reference to database struct
* @cmd: current cmd struct to execute
*
* Return: 0 on success, else error code is returned
*/
int op_write(db_t *db, arg_t *arg)
{
	int saved, fd, code;

	saved = dup(1);
	fd = open(arg->next->av[0], (O_WRONLY | O_CREAT | O_TRUNC));
	dup2(fd, 1);

	code = execute_arg(db, arg);

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
int op_append(db_t *db, arg_t *arg)
{
	int saved, fd, code;

	saved = dup(1);
	fd = open(arg->next->av[0], (O_WRONLY | O_CREAT | O_APPEND));
	dup2(fd, 1);

	code = execute_arg(db, arg);

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
int op_read(db_t *db, arg_t *arg)
{
	int saved, fd, code;

	saved = dup(0);
	fd = open(arg->next->av[0], O_RDONLY);
	if (fd == -1)
		return (eprint(READ_ERR, db, arg->next->av));

	dup2(fd, 0);

	code = execute_arg(db, arg);

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
int op_heredoc(db_t *db, arg_t *arg)
{
	int i;
	arg_t *current;

	(void)db;

	current = arg;
	while (current != NULL)
	{
		for (i = 0; current->av[i] != NULL; i++)
			printf("%d: [%s]\n", i, current->av[i]);
		current = current->next;
	}
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
int op_pipe(db_t *db, arg_t *arg)
{
	int i;
	arg_t *current;

	(void)db;

	current = arg;
	while (current != NULL)
	{
		for (i = 0; current->av[i] != NULL; i++)
			printf("%d: [%s]\n", i, current->av[i]);
		current = current->next;
	}
	printf("For use with [|]\n");

	return (1);
/*	int status, left_flag = 0, right_flag = 0;
	char *left = NULL, *right = NULL;

	db->env = format_env(db);
	if (db->env == NULL)
		return (eprint(MALLOC_ERR, db, NULL));

	status = check_path(db, cmd->left[0], &left);
	if (status == -1 && errno != ENOMEM)
		return (eprint(PATH_ERR, db, cmd->left));
	if (status == -2 || errno == ENOMEM)
		return (eprint(MALLOC_ERR, db, cmd->left));

	status = check_path(db, cmd->right[0], &right);
	if (status == -1 && errno != ENOMEM)
		return (eprint(PATH_ERR, db, cmd->right));
	if (status == -2 || errno == ENOMEM)
		return (eprint(MALLOC_ERR, db, cmd->right));

	if (left == NULL)
	{
		left = cmd->left[0];
		left_flag = 1;
	}
	if (right == NULL)
	{
		right = cmd->right[0];
		right_flag = 1;
	}

	status = execute_pipe(db, cmd, left, right);

	if (left_flag == 0)
		free(left);
	if (right_flag == 0)
		free(right);

	return (status);
*/
}
