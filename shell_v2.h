#ifndef _SHELL_V2_
#define _SHELL_V2_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

struct listcmd_s;

typedef struct cmd_s
{
	char **left;
	char **right;
	char *psep;
	int (*opf)(struct listcmd_s *, struct cmd_s *);
	struct cmd_s *next;
} cmd_t;

typedef struct listcmd_s
{
	cmd_t *head;
	char **env;
	int pstat;
} listcmd_t;

typedef struct gum_s
{
	char *op;
	int (*f)(listcmd_t *, cmd_t *);
} gum_t;

/* listcmd_t Functions */
listcmd_t *build_cmds(char *, char **);
char *gen_cmds(listcmd_t *, char *, int);
void execute_list(listcmd_t *);
void *free_listcmd(listcmd_t *);

/* cmd_t Functions */
cmd_t *build_cmd(char *, char *);
int (*gumball(char *))(listcmd_t *, cmd_t *);
void *free_cmd(cmd_t *);
void execute_cmd(listcmd_t *, char **);

/* Redirect Functions */
int op_write(listcmd_t *, cmd_t *);
int op_append(listcmd_t *, cmd_t *);
int op_read(listcmd_t *, cmd_t *);
int op_heredoc(listcmd_t *, cmd_t *);
int op_pipe(listcmd_t *, cmd_t *);

/* Separate Functions */
int op_semi(listcmd_t *, cmd_t *);
int op_or(listcmd_t *, cmd_t *);
int op_and(listcmd_t *, cmd_t *);

/* String Functions */
int _strcmp(char *, char *);
char *_strdup(char *);

/* Debug Functions */
int example(listcmd_t *, cmd_t *);

#endif /* _SHELL_V2_ */
