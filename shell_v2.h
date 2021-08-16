#ifndef _SHELL_V2_
#define _SHELL_V2_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdarg.h>
#include <fcntl.h>

/**
* struct env_s - linked list representation of env
* @s: key=value string
* @next: next node
*/
typedef struct env_s
{
	char *s;
	struct env_s *next;
} env_t;

/**
* struct arg_s - linked list containing tokenized command args
* @av: tokenized command args, NULL terminated. for use with execve
* @path: absolute path of command to be plugged into execve
* @check_path: status code of path, -1 for path fail, 1 for given, else 0
* @next: next node
*/
typedef struct arg_s
{
	char **av;
	char *path;
	int check_path;
	struct arg_s *next;
} arg_t;

/**
* struct db_s - database representing the current environment
* @pname: program name
* @ln: line number used for error printing
* @pstat: most recent command's exit code
* @toexit: flag to be set when an exit is ordered
* @env: array of environment variables for use with execve
* @a_max: the max number of variables that can fit in env currently
* @envh: head of a linked list representation of the environment variables
* @h_size: the current number of nodes in the envh linked list
* @h_diff: flag set to mark that envh has been updated
* @pid: string representation of the shell's process ID
* @vstat: string representation of pstat
*/
typedef struct db_s
{
	char *pname;
	int ln;
	int pstat;
	int toexit;
	char **env;
	int a_max;
	env_t *envh;
	int h_size;
	int h_diff;
	char *pid;
	char *vstat;
} db_t;

/**
* struct cmd_s - linked list node that holds all info for command execution
* @head: linked list of argument groups
* @psep: function reference to the separate operator
* @opf: function reference to the redirect operator
* @next: reference to next cmd in the linked list
*/
typedef struct cmd_s
{
	arg_t *head;
	int (*psep)(int);
	int (*opf)(db_t *, arg_t *);
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
	char op;
	int (*f)(db_t *, arg_t *);
} rball_t;

/**
* struct sball_s - pair of operator and function
* @op: separator operator
* @f: function reference related to the operator
*/
typedef struct sball_s
{
	char op;
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

/* Pipe Macros */
#define READ 0
#define WRITE 1

/* Custom Error Macros */
#define MALLOC_ERR 0
#define SETENV_ERR1 1
#define SETENV_ERR2 2
#define UNSET_ERR1 3
#define UNSET_ERR2 4
#define EXIT_ERR 5
#define CD_ERR 6
#define DB_ERR 7
#define PATH_ERR 8
#define READ_ERR 9

int handle_vars(arg_t *arg, db_t *db);
void handle_comments(arg_t *arg);

/* Database Functions */
db_t *build_db(char *, char **);
char *dup_atoi(int n);
int rev_env(db_t *, char **);
char **format_env(db_t *);
void *free_db(db_t *);

/* listcmd_t Functions */
listcmd_t *build_cmds(char *, db_t *);
int gen_cmds(listcmd_t *, char *);
void execute_list(listcmd_t *);
void *free_listcmd(listcmd_t *);

/* cmd_t Functions */
cmd_t *build_cmd(db_t *, char *, char);
arg_t *build_arg(char *, db_t *);
int setup_path(arg_t *, db_t *);
int execute_arg(db_t *, arg_t *);
void *free_cmd(cmd_t *);

/* Gumball Functions */
int (*rball(char *))(db_t *, arg_t *);
int (*sball(char))(int);
int (*bball(char *))(db_t *, char **);

/* Builtin Functions */
int bi_exit(db_t *, char **);
int bi_cd(db_t *, char **);
int bi_env(db_t *, char **);
int bi_setenv(db_t *, char **);
int bi_unsetenv(db_t *, char **);

/* Builtin Helpers */
env_t *insert_env(db_t *, char *, char *);
char *get_env(db_t *, char *);

/* Redirect Functions */
int op_write(db_t *, arg_t *);
int op_append(db_t *, arg_t *);
int op_read(db_t *, arg_t *);
int op_heredoc(db_t *, arg_t *);
int op_pipe(db_t *, arg_t *);

/* Separator Functions */
int op_semi(int);
int op_or(int);
int op_and(int);

/* Misc Helpers */
int _strcmp(char *, char *);
char *_strdup(char *);
int _strlen(char *);
int count_words(char *);
void close_all(int (*)[2], int);

/* Error Handling */
int eprint(int, db_t *, char **);
int vfeprint(int code, char *format, ...);

#endif /* _SHELL_V2_ */
