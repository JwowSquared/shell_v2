all: hsh

files = \
cmd_t.c \
helper_cmd_t.c \
engine.c \
gumball.c \
listcmd_t.c \
op_separate.c \
builtins.c \
database.c \
eprint.c \
helper_builtins.c \
op_redirect.c \
helper_misc.c

hsh: $(files)
	gcc -Wall -Werror -Wextra -pedantic $(files) -o hsh

clean:
	rm hsh
