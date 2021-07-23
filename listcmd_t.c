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

	gen_cmds(out, input);

	return (out);
}

/**
* gen_cmds - recursively uses strtok to create a linked list of cmds
* @list: reference to listcmd struct
* @input: raw input line, although clipped of the newline at the end
*
* Return: value only useful within gen_cmds itself; pointer to next token
*/
char *gen_cmds(listcmd_t *list, char *input)
{
	char *tmp, *line;
	int sep = 0;
	cmd_t *cmd;

	tmp = strtok(input, " ");
	if (tmp == NULL)
		return (NULL);
	if (input || !_strcmp(tmp, ";") || !_strcmp(tmp, "||") || !_strcmp(tmp, "&&"))
		sep = 1;
	line = gen_cmds(list, NULL);
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

/**
* execute_list - executes the list of commands inside the listcmd struct
* @list: reference to listcmd struct
*/
void execute_list(listcmd_t *list)
{
	cmd_t *current;

	current = list->head;
	while (current != NULL)
	{
		if (current->opf != NULL)
			current->opf(list->db, current);
		else
			execute_cmd(list->db, current->left);
		current = current->next;
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
