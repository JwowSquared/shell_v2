#include "shell_v2.h"

void test(char *input)
{
	char *temp = strtok(input, ";|&");
	if (temp == NULL)
		return;
	printf("DOWN [%s]\n", temp);
	test(NULL);
	printf("UP [%s]\n", temp);
}

char *test2(char *input, int s)
{
	char *temp, *line;
	int sep = s;

	temp = strtok(input, " ");
	if (temp == NULL)
		return (NULL);
	if (!strcmp(temp, ";") || !strcmp(temp, "||") || !strcmp(temp, "&&"))
		sep = 1;
	line = test2(NULL, 0);
	if (!sep && line != NULL && input == NULL)
		*(line - 1) = ' ';
	if (sep)
	{
		if (input != NULL)
		{
			*(line - 1) = ' ';
			printf("GEN CMD WITH [%s] AND NO OP\n", temp);
		}
		else
			printf("GEN CMD WITH [%s] FOR [%s]\n", line, temp);
		return (NULL);
	}
	return (temp);
}

int main(int ac, char **av)
{
	(void)ac;

	test2(av[1], 1);

	return (0);
}
