#include "shell_v2.h"

int example(cmd_t *cmd)
{
        int i;

	if (cmd->psep != NULL)
		printf("SEPERATOR = [%s]\n", cmd->psep);

        for (i = 0; cmd->left[i] != NULL; i++)
                printf("%d: [%s]\n", i, cmd->left[i]);

        for (i = 0; cmd->right[i] != NULL; i++)
                printf("%d: [%s]\n", i, cmd->right[i]);

        printf("For use with [%s]\n", cmd->op);

        return (1);
}

listcmd_t *build_cmds(char *input)
{
	listcmd_t *out;

	out = malloc(sizeof(listcmd_t));
	if (out == NULL)
		return (NULL);
	out->pstat = 0;
	out->head = NULL;
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
	if (input != NULL || (!sep && line != NULL))
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

void exe_list(listcmd_t *list)
{
	cmd_t *current;

	current = list->head;
	while (current != NULL)
	{
		current->opf(current);
		current = current->next;
	}
}

int main(int ac, char **av)
{
	listcmd_t *list;

	(void)ac;

	list = build_cmds(av[1]);
	exe_list(list);
	free_listcmd(list);

	return (0);
}
