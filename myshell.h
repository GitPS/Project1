/*
 * Ryan Gittins and Phillip Sime
 *
 * CS441: Project 1 Part 2
 * 2013-10-10
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
 *   shell_name : String containing the name of the shell.
 * Return 0 on success, ow error
 */
int start_interactive_shell(char * shell_name);

/*
 * Checks to see if the provided binary is 
 * a built in command or not.
 *   filename : Name of the file that contains commands to execute.
 *   total_jobs : Integer to hold the number of jobs.
 *   total_background_jobs : Integer to hold the number of background jobs.
 *  Return 0 on success, ow error
 */
int start_batch_shell(char *filename, int *total_jobs, int *total_background_jobs);

/*
 * Checks whether the provided string is a built-in command.
 *   binary : String containing the command to execute without parameters.
 * Return 1 if built in job, 0 otherwise. 
 */
int is_built_in_command(char * binary);

/*
 * Executes a foreground job.
 *   binary : String containing the command to execute without parameters.
 *   argc : Integer containing the total number of arguments for a specific job.
 *   argv : String array containing the arguments of the job to execute. 
 * Return 0 on success, ow error 
 */
int execute_foreground_job(char * binary, int argc, char ** argv);

/*
 * Executes a background job.
 *   binary : String containing the command to execute without parameters.
 *   argc : Integer containing the total number of arguments for a specific job.
 *   argv : String array containing the arguments of the job to execute. 
 * Return pid_t on successful creation of a new process 
 */
pid_t execute_background_job(char * binary, int argc, char ** argv);


#endif /* MYSHELL_H */
