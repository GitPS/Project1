/*
 * Ryan Gittins and Phillip Sime
 *
 * CS441: Project 1 Part 1
 * 2013-10-01
 *
 */
#include "support.h"

int split_input_into_jobs(char *input_str, int *num_jobs, job_t **loc_jobs)
{
	/* Make an array of job_types in the order which they occur so we can assgn them below */
	job_type* types = malloc(sizeof(job_type));
	
	int i = 0;
	int jobs = 0;
    while(input_str[i] != '\0' && input_str[i] != '\n') {
        if(input_str[i] == ';') {
			types[jobs] = JOB_FOREGROUND;
			types = realloc(types, sizeof(types) + sizeof(job_type));
			jobs++;
		}
		else if(input_str[i] == '&') {
			types[jobs] = JOB_BACKGROUND;
			types = realloc(types, sizeof(types) + sizeof(job_type));
			jobs++;
		}
		i++;
    }
	
	/* Add an extra foreground job in case the last job has no trailing separator */
	types = realloc(types, sizeof(types) + sizeof(job_type));
	types[jobs+1] = JOB_FOREGROUND;
	
	/* NOTE: The above section may be better off relegated to its own 'get_job_assignments(char *input_str)' function, discuss later */
	
	char * str_ptr  = NULL;

    /* Start counting at 0 */
    (*num_jobs) = 0;
	
    /* Split by ';' and '&' */
    for( str_ptr = strtok(input_str, "&;");
         NULL   != str_ptr;
         str_ptr = strtok(NULL, "&;") ) {

        /*
         * Make a place for the new job in the local jobs array
         */
        (*loc_jobs) = (job_t *)realloc((*loc_jobs), (sizeof(job_t) * ((*num_jobs)+1)));
        if( NULL == (*loc_jobs) ) {
            fprintf(stderr, "Error: Failed to allocate memory! Critical failure on %d!", __LINE__);
            exit(-1);
        }

        /* Initialize the job_t structure */
        (*loc_jobs)[(*num_jobs)].full_command = strdup(str_ptr);
        (*loc_jobs)[(*num_jobs)].argc = 0;
        (*loc_jobs)[(*num_jobs)].argv = NULL;
        
		/* Assign the foreground/background status (as determined above) to the job */
        (*loc_jobs)[(*num_jobs)].type = types[(*num_jobs)];

        /* Increment the number of jobs */
        (*num_jobs)++;
    }

    /* Note: May need to add code here to check for forground/background */

    return 0;
}

int split_job_into_args(job_t *loc_job)
{
    char * str_ptr  = NULL;

    /* Start counting at 0 */
    loc_job->argc = 0;

    /* Split by ' ' */
    for( str_ptr = strtok(loc_job->full_command, " ");
         NULL   != str_ptr;
         str_ptr = strtok(NULL, " ") ) {
         
        /* Strip off the new line */
        if( '\n' == str_ptr[ strlen(str_ptr) - 1] ) {
            str_ptr[ strlen(str_ptr) - 1] = '\0';
        }

        /*
         * Make a place for the new argument in the argv array
         * +1 for NULL termination in the sizeof calculation below (for execvp)
         */
        loc_job->argc++;
        loc_job->argv = (char **)realloc(loc_job->argv, (sizeof(char*) * ((loc_job->argc)+1)));
        if( NULL == loc_job->argv ) {
            fprintf(stderr, "Error: Failed to allocate memory! Critical failure on %d!", __LINE__);
            exit(-1);
        }

        /* Copy over the argument */
        loc_job->argv[(loc_job->argc)-1] = strdup(str_ptr);
        loc_job->argv[loc_job->argc]     = NULL;
    }

    return 0;
}
