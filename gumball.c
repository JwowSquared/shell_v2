#include "shell_v2.h"

/**
* rball - returns the proper function based on the given redirect
* @op: string to compare against the gumballs
*
* Return: function pointer that handles that specific operator
*/
int (*rball(char *op))(db_t *, cmd_t *)
{
	int i;
	rball_t pot[] = {
		{">", &op_write},
		{">>", &op_append},
		{"<", &op_read},
		{"<<", &op_heredoc},
		{"|", &op_pipe},
		{NULL, NULL}
	};

	if (op == NULL)
		return (NULL);

	for (i = 0; pot[i].op != NULL; i++)
		if (!_strcmp(pot[i].op, op))
			return (pot[i].f);

	return (NULL);
}

/**
* sball - returns the proper function based on the given separator
* @op: string to compare against the gumballs
*
* Return: function pointer that handles that specific operator
*/
int (*sball(char *op))(int)
{
	int i;
	sball_t pot[] = {
		{";", &op_semi},
		{"||", &op_or},
		{"&&", &op_and},
		{NULL, NULL}
	};

	if (op == NULL)
		return (NULL);

	for (i = 0; pot[i].op != NULL; i++)
		if (!_strcmp(pot[i].op, op))
			return (pot[i].f);

	return (NULL);
}

/**
* bball - returns the proper function based on the given builtin
* @cmd: string to compare against the gumballs
*
* Return: function pointer that handles that specific builtin
*/
int (*bball(char *cmd))(db_t *, char **)
{
	int i;
	bball_t pot[] = {
		{"exit", &bi_exit},
		{"cd", &bi_cd},
		{"env", &bi_env},
		{"setenv", &bi_setenv},
		{"unsetenv", &bi_unsetenv},
		{NULL, NULL}
	};

	if (cmd == NULL)
		return (NULL);

	for (i = 0; pot[i].cmd != NULL; i++)
		if (!_strcmp(pot[i].cmd, cmd))
			return (pot[i].f);

	return (NULL);
}
