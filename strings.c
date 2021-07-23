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

/**
* _atoi - converts string number into integer, for use with bi_exit
* @code: string to convert
* @db: used for error printing, database of current environment
*
* Return: converted integer, else 2 when invalid string is given
*/
int _atoi(char *code, db_t *db)
{
	int result = 0, i = 0, tmp;
	char *ep = "%s: %s: %s: numeric argument is required\n";

	while (code[i] != '\0')
	{
		tmp = code[i] - '0';
		if (tmp < 0 || tmp > 9)
		{
			fprintf(stderr, ep, db->pname, "exit", code);
			return (2);
		}
		result *= 10;
		result += tmp;
		i++;
	}

	return (result);
}
