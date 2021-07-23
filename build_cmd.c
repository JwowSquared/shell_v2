#include "shell_v2.h"

/**
* gumball - returns the proper function based on the given operator
* @op: string to compare against the gumballs
*
* Return: function that takes a cmd_t pointer and returns an int
*/
int (*gumball(char *op))(cmd_t *)
{
	int i;
	gum_t gm[] = {
		{">", &example},
		{">>", &example},
		{"<", &example},
		{"<<", &example},
		{"|", &example},
		{";", &example},
		{"&&", &example},
		{"||", &example},
		{NULL, NULL}
	};

	for (i = 0; gm[i].op != NULL; i++)
		if (!_strcmp(gm[i].op, op))
			return (gm[i].f);

	return (NULL);
}

/**
* build_cmd - converts a command line into a cmd struct
* @line: command line to convert
*
* Return: pointer to new struct, else NULL
*/
cmd_t *build_cmd(char *line, char *psep)
{
	cmd_t *out;
	char *temp, *word;
	int w_left = 1, w_right = 0, i;

	out = malloc(sizeof(cmd_t));
	if (out == NULL)
		return (NULL);
	out->opf = NULL;
	out->psep = psep;
	line = strtok(line, "\n");
	temp = _strdup(line);
	word = strtok(temp, " ");
	while ((word = strtok(NULL, " ")))
	{
		out->opf = gumball(word);
		if (out->opf != NULL)
			break;
		w_left++;
	}
	while ((word = strtok(NULL, " ")))
		w_right++;
	free(temp);

	out->left = malloc(sizeof(char *) * (w_left + 1));
	if (out->left == NULL)
		return (free_cmd(out));
	out->right = malloc(sizeof(char *) * (w_right + 1));
	if (out->right == NULL)
		return (free_cmd(out));

	out->left[0] = strtok(line, " ");
	for (i = 1; i < w_left; i++)
		out->left[i] = strtok(NULL, " ");
	out->left[i] = NULL;
	out->op = strtok(NULL, " ");
	for (i = 0; i < w_right; i++)
		out->right[i] = strtok(NULL, " ");
	out->right[i] = NULL;

	return (out);
}

/**
* free_cmd - frees the cmd_t struct
* @cmd: pointer to struct to free
*
* Return: always NULL
*/
void *free_cmd(cmd_t *cmd)
{
	if (cmd == NULL)
		return (NULL);

	if (cmd->left != NULL)
		free(cmd->left);
	if (cmd->right != NULL)
		free(cmd->right);

	free(cmd);

	return (NULL);
}
