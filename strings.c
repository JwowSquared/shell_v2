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

	if (s1 == NULL || s2 == NULL)
		printf("_strcmp was passed a null string\n");

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
