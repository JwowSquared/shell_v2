#include "shell_v2.h"

/**
* op_write - > redirect
* @db: reference to database struct
* @arg: current arg list head to execute
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
* @arg: current arg list head to execute
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
* @arg: current arg list head to execute
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
* @arg: current arg list head to execute
*
* Return: 0 on success, else error code is returned
*/
int op_heredoc(db_t *db, arg_t *arg)
{
	char *line = NULL;
	size_t len = 0;
	int size, status, pipefd[2], interactive = isatty(0);

	pipe(pipefd);
	while (1)
	{
		if (interactive)
			printf("> ");
		size = getline(&line, &len, stdin);
		if (size == -1)
			break;
		line[size - 1] = '\0';
		if (!_strcmp(line, arg->next->av[0]))
			break;
		line[size - 1] = '\n';
		write(pipefd[WRITE], line, size);
	}
	free(line);
	if (!fork())
	{
		close(pipefd[WRITE]);
		dup2(pipefd[READ], STDIN_FILENO);
		close(pipefd[READ]);
		execve(arg->path, arg->av, db->env);
		perror(NULL);
		_exit(2);
	}
	close(pipefd[WRITE]);
	close(pipefd[READ]);
	wait(&status);
	return (WEXITSTATUS(status));
}

/**
* op_pipe - | redirect
* @db: reference to database struct
* @arg: current arg list head to execute
*
* Return: 0 on success, else error code is returned
*/
int op_pipe(db_t *db, arg_t *arg)
{
	int status, (*pipes)[2];
	int num_pipes = 0, i = 0, j, code = 0;
	arg_t *current;

	db->env = format_env(db);
	if (db->env == NULL)
		return (eprint(MALLOC_ERR, db, NULL));
	for (current = arg; current != NULL; num_pipes++)
		current = current->next;
	pipes = malloc(sizeof(int[2]) * num_pipes);
	if (pipes == NULL)
		return (eprint(MALLOC_ERR, db, NULL));
	for (j = 0; j < num_pipes; j++)
		pipe(pipes[j]);
	for (current = arg; i < num_pipes; i++)
	{
		if (current->check_path == -1)
			code = eprint(PATH_ERR, db, current->av);
		else if (!fork())
		{
			if (i != 0)
				dup2(pipes[i - 1][READ], STDIN_FILENO);
			if (i + 1 != num_pipes)
				dup2(pipes[i][WRITE], STDOUT_FILENO);
			close_all(pipes, num_pipes);
			execve(current->path, current->av, db->env);
			perror(NULL);
			_exit(2);
		}
		if (i != 0)
			close(pipes[i][READ]);
		close(pipes[i][WRITE]);
		if (wait(&status) > 0)
			code = WEXITSTATUS(status);
		current = current->next;
	}
	free(pipes);
	return (code);
}
