/*
 * Ryan Gittins and Phillip Sime
 *
 * CS441: Project 1 Part 2
 * 2013-10-01
 *
 */
#include "myshell.h"

int main(int argc, char * argv[]) {

	char shell_name[16] = "myshell$";
	
	// Start shell in interactive mode.
	if(argc < 2){
		start_interactive_shell(shell_name);		
	}	
	
	// Start shell in batch mode.
	else{
		int i;
		int total_jobs = 0;
		int total_background_jobs = 0;

		for( i = 1; i < argc; ++i ) {
			// Process next file.
		    start_batch_shell(argv[i], &total_jobs, &total_background_jobs);
		}
		
		printf("Total number of jobs: %d\n", total_jobs);
		printf("Total number of background jobs: %d\n", total_background_jobs);
	}
	
    return 0;
}

int start_interactive_shell(char * shell_name){
	char *buffer = NULL;
	char *fgets_rtn = NULL;
	int num_jobs = 0;
	int job_number = 0;
	int total_jobs = 0;
	int total_background_jobs = 0;
	int i, j;
	job_t *loc_jobs = NULL;
	
	loc_jobs = (job_t*)malloc(sizeof(job_t) * 1);
	if( NULL == loc_jobs ) {
        fprintf(stderr, "Error: Failed to allocate memory! Critical failure on %d!", __LINE__);
        exit(-1);
    }
	
	buffer = (char*) malloc( sizeof(char) * LINELEN);
	if(NULL == buffer){
		fprintf(stderr, "Error: Cannot allocate memory\n");
		exit(-1);
	}
	
	// Loop continously asking for input until exit case is met.
	do{
		i = 0;
		j = 0;
		
		printf("%s ", shell_name);
		fgets_rtn = fgets(buffer, LINELEN, stdin);
		
		// If Ctrl-D is entered, program will interpret this as an exit command.
		if(fgets_rtn == NULL){
			fgets_rtn = strndup("exit", (sizeof(char) * 4));
		}
		
		split_input_into_jobs(fgets_rtn, &num_jobs, &loc_jobs);
		
		/*
		 * For each of those jobs split out the argument set
		 */
		for( i = 0; i < num_jobs; ++i ) {
			char *binary;

			split_job_into_args( &(loc_jobs[i]) );
			binary = strdup(loc_jobs[i].argv[0]);

			fflush(NULL);
			// Display the binary.
			if(is_built_in_command(binary) == 1){
				printf("Job %d^: <%s>", job_number + 1, binary);
				// Check for exit command.
				if(strncmp("exit", binary, strlen(binary)) == 0){
					fgets_rtn = NULL;
					if(binary != NULL){
						free(binary);
						binary = NULL;
					}					
					// Exit the loop here.
					break;
				}	
			}
			else{
				if(loc_jobs[i].type == JOB_BACKGROUND){
					printf("Job %d*: <%s>", job_number + 1, binary);
					total_jobs++;
					total_background_jobs++;
				} else if(loc_jobs[i].type == JOB_FOREGROUND){
					// TEST START
					execute_foreground_job(binary, loc_jobs[i].argc, loc_jobs[i].argv);
					// TEST END
					total_jobs++;
				}
				else {
					fprintf(stderr, "Error: Failed to assign job type for Job %d: <%s>! Critical failure on %d!", job_number, binary, __LINE__);
					exit(-1);
				}
			}
			
			// Increase job count each time we print a job.
			job_number++;
			// Remove this new line when the background jobs are implemented. 
			printf("\n");
			fflush(NULL);
			if(binary != NULL){
				free(binary);
				binary = NULL;
			}
		}
	} while(fgets_rtn != NULL);
	
	printf("\n");
	printf("Total number of jobs: %d\n", total_jobs);
	printf("Total number of background jobs: %d\n", total_background_jobs);
	
	if(buffer != NULL){
		free(buffer);
		buffer = NULL;
	}
	
	/*
     * Cleanup
     */
    if( NULL != loc_jobs ) {
        /* Cleanup struct fields */
        for( i = 0; i < num_jobs; ++i ) {
            /* .full_command */
            if( NULL != loc_jobs[i].full_command ) {
                free( loc_jobs[i].full_command );
                loc_jobs[i].full_command = NULL;
            }

            /* .argv */
            if( NULL != loc_jobs[i].argv ) {
                for( j = 0; j < loc_jobs[i].argc; ++j ) {
                    if( NULL != loc_jobs[i].argv[j] ) {
                        free( loc_jobs[i].argv[j] );
                        loc_jobs[i].argv[j] = NULL;
                    }
                }
                free( loc_jobs[i].argv );
                loc_jobs[i].argv = NULL;
            }

            /* .argc */
            loc_jobs[i].argc = 0;
            
            /* .type */
            loc_jobs[i].type = JOB_NULL;
        }

        /* Free the array */
        free(loc_jobs);
        loc_jobs = NULL;
    }
	
	return 0;		
}

int start_batch_shell(char *filename, int *total_jobs, int *total_background_jobs){
	FILE *fd = NULL;
	char *buffer = NULL;
	char *fgets_rtn = NULL;
	int num_jobs = 0;
	int job_number = 0;
	int i, j;
	job_t *loc_jobs = NULL;
	
	// Open file
	fd = fopen(filename, "r");
	if(NULL == fd){
		fprintf(stderr, "Error: Cannot open the file %s for reading!\n", filename);
		return -1;
	}
	
	loc_jobs = (job_t*)malloc(sizeof(job_t) * 1);
	if( NULL == loc_jobs ) {
        fprintf(stderr, "Error: Failed to allocate memory! Critical failure on %d!", __LINE__);
        exit(-1);
    }
	
	buffer = (char*) malloc( sizeof(char) * LINELEN);
	if(NULL == buffer){
		fprintf(stderr, "Error: Cannot allocate memory\n");
		exit(-1);
	}
	
	// Loop continously asking for input until exit case is met.
	while ( 0 == feof(fd) ){
		i = 0;
		j = 0;
		
		fgets_rtn = fgets(buffer, LINELEN, fd);
		if(fgets_rtn == NULL){
			break;
		}
		
		split_input_into_jobs(fgets_rtn, &num_jobs, &loc_jobs);
		
		/*
		 * For each of those jobs split out the argument set
		 */
		for( i = 0; i < num_jobs; ++i ) {
			char *binary;

			split_job_into_args( &(loc_jobs[i]) );
			binary = strdup(loc_jobs[i].argv[0]);

			fflush(NULL);
			// Display the binary.
			if(is_built_in_command(binary) == 1){
				printf("Job %d^: <%s>", job_number + 1, binary);
				// Check for exit command.
				if(strncmp("exit", binary, strlen(binary)) == 0){
					fgets_rtn = NULL;
					if(binary != NULL){
						free(binary);
						binary = NULL;
					}					
					// Print a new line to keep correct formatting.
					printf("\n");
					// Exit the loop here.
					break;
				}	
			}
			else{
				if(loc_jobs[i].type == JOB_BACKGROUND){
					printf("Job %d*: <%s>", job_number + 1, binary);
					*total_jobs = *total_jobs + 1;
					*total_background_jobs = *total_background_jobs + 1;
				} else if(loc_jobs[i].type == JOB_FOREGROUND){
					printf("Job %d : <%s>", job_number + 1, binary);
					*total_jobs = *total_jobs + 1;
				}
				else {
					fprintf(stderr, "Error: Failed to assign job type for Job %d: <%s>! Critical failure on %d!", job_number, binary, __LINE__);
					exit(-1);
				}
			}
			
			// Print each argument after the binary.
			for( j = 1; j < loc_jobs[i].argc; ++j ) {
				printf(" [%s]", loc_jobs[i].argv[j]);
			}
			// Increase job count each time we print a job.
			job_number++;
			printf("\n");
			fflush(NULL);
			if(binary != NULL){
				free(binary);
				binary = NULL;
			}
		}
	}
	
	if(buffer != NULL){
		free(buffer);
		buffer = NULL;
	}
	
	/*
     * Cleanup
     */
    if( NULL != loc_jobs ) {
        /* Cleanup struct fields */
        for( i = 0; i < num_jobs; ++i ) {
            /* .full_command */
            if( NULL != loc_jobs[i].full_command ) {
                free( loc_jobs[i].full_command );
                loc_jobs[i].full_command = NULL;
            }

            /* .argv */
            if( NULL != loc_jobs[i].argv ) {
                for( j = 0; j < loc_jobs[i].argc; ++j ) {
                    if( NULL != loc_jobs[i].argv[j] ) {
                        free( loc_jobs[i].argv[j] );
                        loc_jobs[i].argv[j] = NULL;
                    }
                }
                free( loc_jobs[i].argv );
                loc_jobs[i].argv = NULL;
            }

            /* .argc */
            loc_jobs[i].argc = 0;
            
            /* .type */
            loc_jobs[i].type = JOB_NULL;
        }

        /* Free the array */
        free(loc_jobs);
        loc_jobs = NULL;
    }
	return 0;	
}

int is_built_in_command(char * binary){
	if(strncmp("exit", binary, strlen("exit")) == 0 || strncmp("jobs", binary, strlen("jobs")) == 0){
		return 1;	
	}
	return 0;
}

int execute_foreground_job(char * binary, int argc, char ** argv){
	char **args = NULL;
	pid_t c_pid = 0;
	int status = 0;
	
	args = (char **) malloc(sizeof(char *) * argc);
	
	// Add the binary to the array.
	args[0] = strdup(binary);
	
	int i;
	// Add the agruments to the array.
	for(i = 1; i < (argc - 1); i++){
		args[i] = strdup(argv[i]);
	}
	// Null in last position.
	args[i] = NULL;
	
	// Fork a process
    c_pid = fork();

    // If there was an error
    if( c_pid < 0 ) {
        fprintf(stderr, "Error: fork failed!\n");
        return -1;
    }
    // Child process
    else if( 0 == c_pid ) {
        execvp(binary, args);
        fprintf(stderr, "Error: Exec failed!\n");
        exit(-1);
    }
    // Parent process
    else {
        // Wait for child
        waitpid(c_pid, &status, 0);
    }

    if( NULL != args ) {
        // Free each string created by strdup() above
        for( i = 0; NULL != args[i]; ++i ) {
            free(args[i]);
        }
        free(args);
    }
	
	return 0;
}







