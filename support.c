/*
 * Ryan Gittins and Phillip Sime
 *
 * CS441: Project 1 Part 2
 * 2013-10-10
 *
 */
#include "support.h"

int split_input_into_jobs(char *input_str, int *num_jobs, job_t **loc_jobs)
{
	/* Make an array of job_types in the order which they occur so we can assgn them below */
	
	// Default array size.
	int job_array_size = 10;
	// Initialize job_type array.
	job_type *types = malloc(sizeof(job_type) * job_array_size);
	
	int j;
	// Set each element in the job_type array to JOB_NULL.
	for(j = 0; j < job_array_size; j++){
		types[j] = JOB_NULL;
	}
	
	int i = 0;
	int jobs = 0;
    while(input_str[i] != '\0' && input_str[i] != '\n') {
        if(input_str[i] == ';') {
			types[jobs] = JOB_FOREGROUND;
			jobs++;
		}
		else if(input_str[i] == '&') {
			types[jobs] = JOB_BACKGROUND;
			jobs++;
		}
		
		/* If the types array is full, double its size and set all new elements to JOB_NULL */
		if(job_array_size <= jobs){
			types = realloc(types, sizeof(job_type) * jobs * 2);
			
			for(j = jobs; j < job_array_size; j++) {
				types[j] = JOB_NULL;
			}
		}
		
		i++;
    }
	
	/* Add an extra foreground job in case the last job has no trailing separator */
	types[jobs] = JOB_FOREGROUND;
	
	/* NOTE: The above section may be better off relegated to its own 'get_job_assignments(char *input_str)' function, discuss later */
	
	char * str_ptr  = NULL;

    /* Start counting at 0 */
    (*num_jobs) = 0;
	
    /* Split by ';' and '&' */
    for( str_ptr = strtok(input_str, "&;");
         NULL   != str_ptr && *str_ptr != '\n';
         str_ptr = strtok(NULL, "&;") ) {
		
		if(!is_whitespace(str_ptr)) {
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
    }
    
    /* Cleanup */
	if(types != NULL){
		for(i = 0; i < jobs; i++){
			types[i] = JOB_NULL;		
		}
		free(types);
	}

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

int is_whitespace(char *str)
{
	int i = 0;
	while(str[i] != '\0') {
		if(str[i] != ' ') {
			return 0;
		}
		i++;
	}
	return 1;
}

char *trim(char *str)
{
	char *end;

	if(is_whitespace(str)){
		return str;
	}

	// Leading spaces
	while((*str) == ' '){
		str++;
	}

	// Trailing spaces
	end = str + strlen(str) - 1;
	while((*end) == ' '){
		end--;
	}

	// Replace last space with null to terminate
	*(end + 1) = 0;

	return str;
}
