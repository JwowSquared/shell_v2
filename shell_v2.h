#ifndef _SHELL_V2_
#define _SHELL_V2_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* struct db_s - database representing the current environment
* @env: current environment variables
* @pname: program name
* @toexit: flag to be set when an exit is ordered
* @pstat: most recent command's exit code
*/
typedef struct db_s
{
	char **env;
	char *pname;
	int toexit;
	int pstat;
} db_t;

/**
* struct cmd_s - linked list node that holds all info for command execution
* @left: tokenized array of args left of redirect operator
* @right: tokenized array of args right of redirect operator
* @psep: function reference to the separate operator
* @opf: function reference to the redirect operator
* @next: reference to next cmd in the linked list
*/
typedef struct cmd_s
{
	char **left;
	char **right;
	int (*psep)(int);
	int (*opf)(db_t *, struct cmd_s *);
	struct cmd_s *next;
} cmd_t;

/**
* struct listcmd_s - container that makes it easier to pass around references
* @head: reference to the head of a linked list of commands
* @db: reference to database struct representing the current environment
*/
typedef struct listcmd_s
{
	cmd_t *head;
	db_t *db;
} listcmd_t;

/**
* struct rball_s - pair of operator and function
* @op: redirect operator
* @f: function reference related to the operator
*/
typedef struct rball_s
{
	char *op;
	int (*f)(db_t *, cmd_t *);
} rball_t;

/**
* struct sball_s - pair of operator and function
* @op: separator operator
* @f: function reference related to the operator
*/
typedef struct sball_s
{
	char *op;
	int (*f)(int);
} sball_t;

/**
* struct bball_s - pair of builtin and function
* @cmd: builtin command
* @f: function reference related to the builtin
*/
typedef struct bball_s
{
	char *cmd;
	int (*f)(db_t *, char **);
} bball_t;




/* listcmd_t Functions */
listcmd_t *build_cmds(char *, db_t *);
char *gen_cmds(listcmd_t *, char *);
void execute_list(listcmd_t *);
void *free_listcmd(listcmd_t *);

/* cmd_t Functions */
cmd_t *build_cmd(char *, char *);
int execute_cmd(db_t *, char **);
void *free_cmd(cmd_t *);

/* Gumball Functions */
int (*rball(char *))(db_t *, cmd_t *);
int (*bball(char *))(db_t *, char **);
int (*sball(char *))(int);

/* Builtin Functions */
int bi_exit(db_t *, char **);
int bi_cd(db_t *, char **);
int bi_env(db_t *, char **);
int bi_setenv(db_t *, char **);
int bi_unsetenv(db_t *, char **);

/* Redirect Functions */
int op_write(db_t *, cmd_t *);
int op_append(db_t *, cmd_t *);
int op_read(db_t *, cmd_t *);
int op_heredoc(db_t *, cmd_t *);
int op_pipe(db_t *, cmd_t *);

/* Separator Functions */
int op_semi(int);
int op_or(int);
int op_and(int);

/* String Functions */
int _strcmp(char *, char *);
char *_strdup(char *);
int _atoi(char *, db_t *);

#endif /* _SHELL_V2_ */
