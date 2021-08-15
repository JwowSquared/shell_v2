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
	int status, (*pipes)[2], num_pipes, i, j;
	arg_t *current;

	db->env = format_env(db);
	if (db->env == NULL)
		return (eprint(MALLOC_ERR, db, NULL));

	current = arg;
	for (num_pipes = 0; current != NULL; num_pipes++)
		current = current->next;

	pipes = malloc(sizeof(int[2]) * num_pipes);
	if (pipes == NULL)
		return (eprint(MALLOC_ERR, db, NULL));

	for (i = 0; i < num_pipes; i++)
		pipe(pipes[i]);

	current = arg;
	for (i = 0; i < num_pipes; i++)
	{
		if (!fork())
		{
			if (i != 0)
				dup2(pipes[i - 1][READ], STDIN_FILENO);
			if (i + 1 != num_pipes)
				dup2(pipes[i][WRITE], STDOUT_FILENO);
			for (j = 0; j < num_pipes; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			if (current->check_path == -1)
				_exit(eprint(PATH_ERR, db, current->av));
			execve(current->path, current->av, db->env);
			perror(NULL);
			_exit(2);
		}
		current = current->next;
	}
	for (i = 0; i < num_pipes; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		wait(&status);
	}
	free(pipes);
	return (WEXITSTATUS(status));
}
