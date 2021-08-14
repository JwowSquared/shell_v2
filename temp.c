#include "shell_v2.h"

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
