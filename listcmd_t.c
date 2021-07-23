#include "shell_v2.h"

void execute_cmd(listcmd_t *list, char **cmd)
{
	int status;

	if (!fork())
	{
		execve(cmd[0], cmd, list->env);
		perror(NULL);
		exit(2);
	}
	wait(&status);
	list->pstat = WEXITSTATUS(status);
}

listcmd_t *build_cmds(char *input, char **env)
{
	listcmd_t *out;

	input = strtok(input, "\n");
	out = malloc(sizeof(listcmd_t));
	if (out == NULL)
		return (NULL);
	out->pstat = 0;
	out->head = NULL;
	out->env = env;

	gen_cmds(out, input, 1);

	return (out);
}

char *gen_cmds(listcmd_t *list, char *input, int s)
{
	char *tmp, *line;
	int sep = 0;
	cmd_t *cmd;

	tmp = strtok(input, " ");
	if (tmp == NULL)
		return (NULL);
	if (input || !_strcmp(tmp, ";") || !_strcmp(tmp, "||") || !_strcmp(tmp, "&&"))
		sep = 1;
	line = gen_cmds(list, NULL, s + sep);
	if (line != NULL && (input != NULL || !sep))
		*(line - 1) = ' ';
	if (sep)
	{
		if (input != NULL)
			cmd = build_cmd(tmp, NULL);
		else
			cmd = build_cmd(line, tmp);
		cmd->next = list->head;
		list->head = cmd;
		return (NULL);
	}
	return (tmp);
}

void *free_listcmd(listcmd_t *list)
{
	cmd_t *current;

	if (list == NULL)
		return (NULL);

	if (list->head != NULL)
		while ((current = list->head))
		{
			list->head = list->head->next;
			free_cmd(current);
		}

	free(list);
	return (NULL);
}

void execute_list(listcmd_t *list)
{
	cmd_t *current;

	current = list->head;
	while (current != NULL)
	{
		if (current->opf != NULL)
			current->opf(list, current);
		else
			execute_cmd(list, current->left);
		current = current->next;
	}
}
