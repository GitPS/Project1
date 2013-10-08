/*
 * Ryan Gittins and Phillip Sime
 *
 * CS441: Project 1 Part 2
 * 2013-10-01
 *
 */
#ifndef MYSHELL_H
#define MYSHELL_H

#include "support.h"

/* For fork, exec, sleep */
#include <sys/types.h>
#include <unistd.h>
/* For waitpid */
#include <sys/wait.h>

/******************************
 * Defines
 ******************************/


/******************************
 * Structures
 ******************************/
 
 /*
  * Structure to hold a single command line argument.
  */
struct argument_t{
    char * argument;
    job_type type;
};

typedef struct argument_t argument_t;

/******************************
 * Global Variables
 ******************************/
 /*
 * Max length of the buffer for fgets
 */
int LINELEN = 1024;


/******************************
 * Function declarations
 ******************************/

/*
 * Begins execution of an interactive shell allowing the user to 
 * enter their commands via a command line.
 */
int start_interactive_shell(char * shell_name);

/*
 * Begins execution of a batch shell allowing the user to 
 * include the file name of a file to execute.
 */
int start_batch_shell(char *filename, int *total_jobs, int *total_background_jobs);

/*
 * Checks to see if the provided binary is 
 * a built in command or not.
 * Returns:
 *   0 if not built in 
 *   1 if built in 
 */
int is_built_in_command(char * binary);

/*
 * Executes a foreground job.
 * Returns:
 *   -1 on error
 *   0 if successful 
 */
int execute_foreground_job(char * binary, int argc, char ** argv); 	


#endif /* MYSHELL_H */
