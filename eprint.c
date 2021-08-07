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
	int c[] = {2, 2, 2, 2, 2, 2, 2, 2, 127};
	char *f[] = {
		"%s: %d: malloc failed, exiting\n",
		"%s: %d: setenv: key cant be missing\n",
		"%s: %d: setenv: key cant contain '='\n",
		"%s: %d: unsetenv: key cant be missing\n",
		"%s: %d: unsetenv: key not found\n",
		"%s: %d: exit: Illegal number: %s\n",
		"%s: %d: cd: can't cd to %s\n",
		"%s: 0: database malloc failed, exiting\n",
		"%s: %d: %s: not found\n",
		NULL
	};

	if (m == MALLOC_ERR)
		db->toexit = 1;

	if (m <= UNSET_ERR2)
		return (vfeprint(c[m], f[m], db->pname, db->ln));
	if (m <= CD_ERR)
		return (vfeprint(c[m], f[m], db->pname, db->ln, cmd[1]));
	if (m == DB_ERR)
		return (vfeprint(c[m], f[m], cmd[0]));
	if (m == PATH_ERR)
		return (vfeprint(c[m], f[m], db->pname, db->ln, cmd[0]));

	return (-1);
}

/**
* vfeprint - prints using vfprintf and returns code
* @code: return value
* @format: format string for vfprintf
*
* Return: code
*/
int vfeprint(int code, char *format, ...)
{
	va_list args;

	va_start(args, format);

	vfprintf(stderr, format, args);

	va_end(args);

	return (code);
}
