#### CS 441 Project 1 ####

__Authors__

Ryan Gittins and Phillip Sime

__Summary__

This is a command line interpreter written in C which implements a subset of the functionality provided by the Unix shell.  This shell can run in two different modes, interactive and batch, and every command can be run as either a background job or a foreground job.

Interactive mode runs like a normal terminal, accepting commands, running them, and returning the user to the prompt either immediately or once that process has finished running, as determined by the job's status as a foreground or background job.

Batch mode is initiated by a call to `./myshell` followed by one or more filenames, each of which contain a command to be run on each line, much like a batch file.  Foreground and background assignments can also be made in batch mode.

Foreground jobs are assigned such status by a semicolon following the command, while background jobs should be called with a trailing ampersand.  These symbols act as delimiters for commands as well as job type designators.  If no character follows the last command, it will run in the foreground.

All commands rely on the underlying system's commands, with the exception on `jobs` and `exit`.

__Build__

To build this software, simply navigate to the directory containing the Makefile, myshell.c, myshell.h, support.c and support.h files on a unix-based machine or virtual machine. Type the command `make` into the terminal to trigger the build. This will cause the gcc build commands listed in the Makefile to execute, compiling the source code into an executable file.

__Usage__



__Test Cases__



__Examples__



__Known Bugs and Problem Areas__

