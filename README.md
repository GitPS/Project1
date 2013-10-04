#### CS 441 Project 1 ####

__Authors__

Ryan Gittins and Phillip Sime
2013-10-03

__Summary__

This is a command line interpreter written in C which implements a subset of the functionality provided by the Unix shell.  This shell can run in two different modes, interactive and batch, and every command can be run as either a background job or a foreground job.

Interactive mode runs like a normal terminal, accepting commands, running them, and returning the user to the prompt either immediately or once that process has finished running, as determined by the job's status as a foreground or background job.

Batch mode is initiated by a call to `./myshell` followed by one or more filenames, each of which contain a command to be run on each line, much like a batch file.  Foreground and background assignments can also be made in batch mode.

Foreground jobs are assigned such status by a semicolon following the command, while background jobs should be called with a trailing ampersand.  These symbols act as delimiters for commands as well as job type designators.  If no character follows the last command, it will run in the foreground.

All commands rely on the underlying system's commands, with the exception of `jobs` and `exit`.

__Build__

To build this software, simply navigate to the directory containing the Makefile, myshell.c, myshell.h, support.c and support.h files on a unix-based machine or virtual machine. Type the command `make` into the terminal to trigger the build. This will cause the gcc build commands listed in the Makefile to execute, compiling the source code into an executable file.

__Usage__

To use this software in interactive mode, simply run the command `./myshell` at the command line.  This will launch the shell, to which you can submit commands.  You can run multiple commands at once, delimited by semicolons or ampersands, as discussed in the summary.  Once you've typed the command, simply press the enter key to execute it.

To use this software in batch mode, use the `./myshell` command again, followed by one or more textfile names, delimited by spaces.  These files should contain commands as you would run them in interactive mode, with one on every line.  An example call would be `./myshell test1.txt test2.txt`.  This mode also supports both foreground and background job types.

__Test Cases__

The test suite includes four textfiles, each of which serves a purpose.  They are as follows:

test1.txt -- This test is the one included on page three of the PDF file.  Its output is identical to that which is to be expected.

test2.txt -- This file is 1000 lines long and contains nearly 1500 jobs.  This demonstrates the program's excellent performance under stress as well as its handling of multiple commands on one line and different job types.

test3.txt -- This test demonstrates some longer test commands with varying numbers of arguments.  These tests are also a demonstration of the shell's handling of some real-world commands a shell might have to parse and run.

test4.txt -- This file contains many blank lines, demonstrating the shell's handling of empty input.

__Examples__



__Known Bugs and Problem Areas__

There are no known bugs at this time.