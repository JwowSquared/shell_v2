#include "shell_v2.h"

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

	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;

	return (s1[i] - s2[i]);
}

/**
* _strdup - duplicates input string
* @input: string to duplicate
*
* Return: pointer to new string, else NULL
*/
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

/**
* _strlen - returns length of string
* @str: string to find length of
*
* Return: length of str, else 0
*/
int _strlen(char *str)
{
	int i = 0;

	if (str == NULL)
		return (0);

	while (str[i])
		i++;

	return (i);
}

/**
* count_words - counts the words in a string
* @line: string with words to count
*
* Return: number of words in line, else 0
*/
int count_words(char *line)
{
	int i = 0, out = 0;

	if (line == NULL || line[0] == '\0')
		return (0);

	while (line[i] == ' ')
		i++;
	while (line[i])
	{
		out++;
		while (line[i] && line[i] != ' ')
			i++;
		while (line[i] && line[i] == ' ')
			i++;
	}
	return (out);
}

/**
* close_all - closes all pipes
* @pipes: double array of pipes to close
* @num_pipes: number of pipe pairs inside pipes
*/
void close_all(int (*pipes)[2], int num_pipes)
{
	int i;

	for (i = 0; i < num_pipes; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}
