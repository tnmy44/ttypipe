#TTYPIPE

Ttypipe allows you to create a pseudo terminal to act as the standard output of a command while you pipeline the actual data somewhere else. This is useful for debugging puposes when the data written to STDOUT by a command depends on whether STDOUT is a terminal or not. Eg. "ttypipe vim > vim.out".

##Compilation

g++ -o ttypipe ttypipe.c

##Usage

ttypipe command arg1 arg2 ...

First argument is treated as command name, and successive arguments are passed to the command.