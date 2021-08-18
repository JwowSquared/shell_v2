# Shell v2
Shell v2 is a project for Holberton which emulates the Unix shell `/bin/sh`.
Over 22 days, I wrote this entire project from scratch.
You can compile via the Makefile, which will create the `hsh` executable.
commands can be piped in such as `echo "ls -la" | ./hsh`, or you can simply run `./hsh` and the program will ask for your input. You can exit the interactive mode with the `exit` command.

## The Development Process
My primary focus was to limit memory usage. In Shell v1, I was very reckless with my memory, copying many things many times as it was the most straight forward for me as a student in early Foundations at Holberton. Now a year later, I knew I wanted to avoid this approach and instead optimize and use minimal memory, even if I spent a lot of extra development time as a result. I used linked lists and pointers to my advantage with great results in this project. Because of this, my shell has about a 2600 byte start up cost to initialize my environment variable list, and only uses about 300 bytes for each simple command, including setenv and unsetenv which were huge road blocks in Shell v1. Thanks to my efforts, Shell v2 uses about 100x less memory than my Shell v1.

## Setbacks
I had to undergo 2 massive rewrites of my core logic. The first was 14 days into the project, where I realized that commands do not necessarily have spaces around separator and redirection operators. My core relied on these spaces, using the strtok() function with space as a delimiter. Thankfully about half of my logic transferred to the new system, where I abandoned my old strtok() strategy and coded a new function that actually used LESS memory despite being more complex. The second rewrite was actually 20 days in, where I noticed that multiple pipe operators could be present in one command. My previous infastructure relied on exactly 1 pipe, and so I had to write a new function that allowed for any number of pipes in a command. My core logic explicitly had a collection of words LEFT of a redirect operator and RIGHT of a redirect operator, but those had to be replaced with a linked list of collections. This rewrite was pretty seamless, but also fairly stressful to be done so close to the deadline. I am very proud of myself for working so dilligently throughout the entire deadline, and to finish 100% of the mandatory tasks feels awesome.

## Cool Things To Check Out
* **eprint.c**
My error handling functions, `eprint` and `vfeprint`. Passing eprint a MACRO error code and using a variadic function toprint the different error cases was a super satisfying solution.
* **gumball.c**
the redirect function pointer "gumball" machine, `rball`. This one is really cool because when the first character of the operator matches, I can move to the appropriate function return by adding 1 to `i` if the second character matches the first character. This is especially cool because two pipes are NOT a redirect operator, and if the second character matches the first, in this case `i` moves forward and returns NULL to indicate a failure to match. The fact that I can generalize the 3 cases while returning something in the first 2 cases and nothing in the third case is really neat.
* **database.c**
My linked list to `char **` converter, `format_env`. The flags required for this really cluttered my database struct, but the efficiency is crazy. This is where the high startup cost but low maintenance cost comes from. I copy the `envp` argument from main into a linked list, which is where I manage additions, modifications and deletions from `setenv` and `unsetenv`. However, `execve` requires a `char **`, and so I allocate memory just large enough to hold the linked list, and I have the pointers in the array just point to the existing strings in my linked list instead of copying the strings. I also maintain a reference to this array memory, and I only reallocate it if the size of my linked list increases past the stored maximum. Shell v1 copied the entire array any time there was a modification, which would incur a huge memory penalty. All modifications besides increasing past the maximum size have little or no memory cost in Shell v2.
* **op_redirect.c**
The two redirects involving pipes, `op_heredoc` and `op_pipe`. Pipes were completely new to me for this project, and getting the heredoc and pipe operators to actually work was super satisfying. Especially my logic that handles any number of pipes.

## Thank You
Yeah, you. Thanks for reading this and checking out my project.
