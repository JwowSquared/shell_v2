#include "shell_v2.h"

/**
* build_cmds - creates a listcmd based on input string
* @input: raw line of input
* @db: reference to database struct
*
* Return: pointer to new struct, else NULL
*/
listcmd_t *build_cmds(char *input, db_t *db)
{
	listcmd_t *out;

	input = strtok(input, "\n");
	out = malloc(sizeof(listcmd_t));
	if (out == NULL)
		return (NULL);
	out->db = db;
	out->head = NULL;

	if (gen_cmds(out, input) == -1)
		return (free_listcmd(out));

	return (out);
}

/**
* gen_cmds - recursively uses strtok to create a linked list of cmds
* @list: reference to listcmd struct
* @input: raw input line, although clipped of the newline at the end
*
* Return: -1 on malloc error, else 0
*/
int gen_cmds(listcmd_t *list, char *input)
{
	int start = 0, i = 0, len;
	char c = '\0', psep = '\0';
	cmd_t *cmd, *prev = NULL;

	len = _strlen(input);
	while (i < len)
	{
		for (start = i; input[i]; i++)
		{
			c = input[i];
			if (c == ';' || c == '&' || (c == '|' && input[i + 1] == '|'))
			{
				input[i] = '\0';
				if (c != ';')
					i++;
				break;
			}
		}
		cmd = build_cmd(&input[start], psep);
		if (cmd == NULL)
			return (-1);
		cmd->next = NULL;
		if (prev != NULL)
			prev->next = cmd;
		else
			list->head = cmd;
		prev = cmd;
		psep = c;
		i++;
	}
	return (0);
}

/**
* execute_list - executes the list of commands inside the listcmd struct
* @list: reference to listcmd struct
*/
void execute_list(listcmd_t *list)
{
	cmd_t *current, *tmp;

	current = list->head;
	while (current != NULL)
	{
		tmp = current;
		current = current->next;

		if (list->db->toexit)
			continue;
		if (tmp->psep != NULL && (*tmp->psep)(list->db->pstat))
			continue;
		if (tmp->opf != NULL)
			tmp->opf(list->db, tmp);
		else
			list->db->pstat = execute_cmd(list->db, tmp->left);
	}
}

/**
* free_listcmd - frees data associated with the listcmd struct
* @list: reference to struct to free
*
* Return: always NULL
*/
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
