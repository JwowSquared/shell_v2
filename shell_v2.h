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
	char **right;
	char *psep;
	int (*opf)(struct cmd_s *);
	struct cmd_s *next;
} cmd_t;

typedef struct listcmd_s
{
	cmd_t *head;
	int pstat;
} listcmd_t;

typedef struct gum_s
{
	char *op;
	int (*f)(cmd_t *);
} gum_t;

/* listcmd_t Functions */
listcmd_t *build_cmds(char *input);
char *gen_cmds(listcmd_t *list, char *input, int s);
void *free_listcmd(listcmd_t *);

/* cmd_t Functions */
cmd_t *build_cmd(char *, char *);
int (*gumball(char *))(cmd_t *);
void *free_cmd(cmd_t *);

/* String Functions */
int _strcmp(char *, char *);
char *_strdup(char *);

/* Debug Functions */
int example(cmd_t *);

#endif /* _SHELL_V2_ */
