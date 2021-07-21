#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shell_v2.h"

/**
 * main - prints arg list
 *
 * Return: Always 0.
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	cmd_t *cmd = NULL;
	int code = 0, interactive = isatty(0);

	while (1)
	{
		if (interactive)
			printf("$ ");
		if (getline(&line, &len, stdin) == -1)
			break;
		cmd = build_cmd(line);
		if (cmd->opf != NULL)
			cmd->opf(cmd);
		else
			printf("execute command normally.\n");
		free_cmd(cmd);
	}

	free(line);

	exit(code);
}

/**
* _strcmp - compares two strings
* @s1: left string
* @s2: right string
*
* Return: zero if strings are equal, else the difference of the first nonmatch
*/
int _strcmp(char *s1, char *s2)
{
	int i = 0;

	if (s1 == NULL || s2 == NULL)
		printf("_strcmp was passed a null string\n");

	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;

	return (s1[i] - s2[i]);
}

char *_strdup(char *input)
{
	char *out;
	int i = 0, j = 0;

	while (input[i])
		i++;

	out = malloc(sizeof(char) * (i + 1));
	if (out == NULL)
		return (NULL);

	while (j < i)
	{
		out[j] = input[j];
		j++;
	}
	out[j] = '\0';

	return (out);
}

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
cmd_t *build_cmd(char *line)
{
	cmd_t *out;
	char *temp, *word;
	int w_left = 1, w_right = 0, i;

	out = malloc(sizeof(cmd_t));
	if (out == NULL)
		return (NULL);

	out->left = NULL;
	out->right = NULL;
	out->op = NULL;
	out->opf = NULL;

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

int example(cmd_t *cmd)
{
	int i;

	for (i = 0; cmd->left[i] != NULL; i++)
		printf("%d: |%s|\n", i, cmd->left[i]);

	for (i = 0; cmd->right[i] != NULL; i++)
		printf("%d: |%s|\n", i, cmd->right[i]);

	printf("For use with |%s|\n", cmd->op);

	return (1);
}
