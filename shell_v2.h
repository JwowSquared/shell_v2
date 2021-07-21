#ifndef _SHELL_V2_
#define _SHELL_V2_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct cmd_s
{
	char **left;
	char *op;
	int (*opf)(struct cmd_s *);
	char **right;
} cmd_t;

typedef struct gum_s
{
	char *op;
	int (*f)(cmd_t *);
} gum_t;

int (*gumball(char *))(cmd_t *);
int _strcmp(char *, char *);
char *_strdup(char *);
cmd_t *build_cmd(char *);
void *free_cmd(cmd_t *);

int example(cmd_t *);

#endif /* _SHELL_V2_ */
