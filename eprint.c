#include "shell_v2.h"

/**
* eprint - handles error messages and codes based on macros
* @m: translated macro
* @db: reference to database struct
* @cmd: current command being executed
*
* Return: proper error code
*/
int eprint(int m, db_t *db, char **cmd)
{
	char *e[] = {
		"%s: %d: malloc failed, exiting\n",
		"%s: %d: exit: Illegal number: %s\n",
		"%s: %d: setenv: key cant be missing\n",
		"%s: %d: setenv: key cant contain '='\n",
		"%s: %d: unsetenv: key cant be missing\n",
		"%s: %d: unsetenv: key not found\n",
		"%s: 0: database malloc failed, exiting\n",
		"%s: %d: cd: cant cd to %s\n",
		"%s: %d: %s: not found\n",
		NULL
	};

	if (m == MALLOC_ERR) /* notably, signals toexit */
	{
		db->toexit = 1;
		fprintf(stderr, e[m], db->pname, db->ln);
		return (2);
	}
	if (m == EXIT_ERR) /* notably, cmd[1] is used */
	{
		fprintf(stderr, e[m], db->pname, db->ln, cmd[1]);
		return (2);
	}
	if (m == CD_ERR) /* notably, cmd[1] is used */
	{
		fprintf(stderr, e[m], db->pname, db->ln, cmd[1]);
		return (2);
	}
	if (m == SETENV_ERR1) /* default */
	{
		fprintf(stderr, e[m], db->pname, db->ln);
		return (2);
	}
	if (m == SETENV_ERR2) /* default */
	{
		fprintf(stderr, e[m], db->pname, db->ln);
		return (2);
	}
	if (m == UNSET_ERR1) /* default */
	{
		fprintf(stderr, e[m], db->pname, db->ln);
		return (2);
	}
	if (m == UNSET_ERR2) /* default */
	{
		fprintf(stderr, e[m], db->pname, db->ln);
		return (2);
	}
	if (m == DB_ERR) /* notably, db is NULL */
	{
		fprintf(stderr, e[m], cmd[0]);
		return (2);
	}
	if (m == PATH_ERR) /* could be default */
	{
		fprintf(stderr, e[m], db->pname, db->ln, cmd[0]);
		return (127);
	}

	return (-1);
}
