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
	int status, (*pipes)[2], i, j, k;
	arg_t *current;

	db->env = format_env(db);
	if (db->env == NULL)
		return (eprint(MALLOC_ERR, db, NULL));

	current = arg;
	for (i = 0; current != NULL; i++)
		current = current->next;

	pipes = malloc(sizeof(int[2]) * i);
	if (pipes == NULL)
		return (eprint(MALLOC_ERR, db, NULL));

	for (j = 0; j < i; j++)
		pipe(pipes[j]);

	current = arg;
	for (j = 0; j < i; j++)
	{
		if (current->check_path == -1)
			status = eprint(PATH_ERR, db, current->av); /* current->path ?? */
		else if (!fork())
		{
			if (j != 0)
				dup2(pipes[j - 1][READ], STDIN_FILENO);
			if (j + 1 != i)
				dup2(pipes[j][WRITE], STDOUT_FILENO);
			for (k = 0; k < i; k++)
			{
				close(pipes[k][0]);
				close(pipes[k][1]);
			}
			execve(current->path, current->av, db->env);
			perror(NULL);
			_exit(2);
		}
		current = current->next;
	}
	for (k = 0; k < i; k++)
	{
		close(pipes[k][0]);
		close(pipes[k][1]);
		wait(&status);
	}
	free(pipes);
	return (WEXITSTATUS(status));
}
