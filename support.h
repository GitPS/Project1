/*
 * Ryan Gittins and Phillip Sime
 *
 * CS441: Project 1 Part 2
 * 2013-10-01
 *
 */
#ifndef SUPPORT_H
#define SUPPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************
 * Defines
 ******************************/
#define TRUE  0
#define FALSE 1

#define MAX_COMMAND_LINE 1024

/******************************
 * Structures
 ******************************/
enum job_type{
    JOB_BACKGROUND,
    JOB_FOREGROUND,
	JOB_NULL
};

typedef enum job_type job_type;

struct job_t {
    char * full_command;
    int argc;
    char **argv;
    job_type type;
};
typedef struct job_t job_t;

struct background_job {
    pid_t c_pid;
    char * full_command;
    int job_number;
    int display;
};
typedef struct background_job background_job;

/******************************
 * Global Variables
 ******************************/


/******************************
 * Function declarations
 ******************************/
/*
 * Split the input string into an array of job_t's
 * Jobs are separated by ';' or '&' characters
 *   input_str : String read from the input stream (may contain multiple jobs)
 *   num_jobs : Number of jobs in the input stream
 *   loc_jobs : Array of job_t's each representing a single job
 * Return 0 on success, ow error
 */
int split_input_into_jobs(char *input_str, int *num_jobs, job_t **loc_jobs);

/*
 * Split the 'full_command' string in the job_t structure into
 * a set of arguments. Arguments are separated by one or more
 * ' ' characters. Upon return the 'argc' and 'argv' fields
 * of the job_t structure are updated appropriately.
 *   loc_job : job_t structure to process
 * Return 0 on success, ow error
 */
int split_job_into_args(job_t *loc_job);

/*
 * Returns 1 if the string is all whitespace, 0 otherwise.
 */
int is_whitespace(char *str);

/* 
 * TODO
 */
char *trim(char *str);

#endif /* SUPPORT_H */
