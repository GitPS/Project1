/*
 * Ryan Gittins and Phillip Sime
 *
 * CS441: Project 1 Part 1
 * 2013-10-01
 *
 */
#include "myshell.h"

int main(int argc, char * argv[]) {

	char shell_name[16] = "myshell$";
	
	// Start shell in interactive mode.
	if(argc < 2){
		start_interactive_shell(shell_name, argv);		
	}	
	
	// Start shell in batch mode.
	else{
		start_batch_shell(shell_name, argv);	
	}
	
    return 0;
}

int start_interactive_shell(char * shell_name, char * argv[]){
	char *buffer = NULL;
	char *fgets_rtn = NULL;
	int num_jobs = 0;
	int job_number = 0;
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
				printf("Job %d^: <%s>", job_number, binary);
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
					printf("Job %d*: <%s>", job_number, binary);	
				} else if(loc_jobs[i].type == JOB_FOREGROUND){
					printf("Job %d : <%s>", job_number, binary);
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
	} while(fgets_rtn != NULL);
	
	printf("\n");
	
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
        }

        /* Free the array */
        free(loc_jobs);
        loc_jobs = NULL;
    }
	
	return 0;		
}

int start_batch_shell(char * shell_name, char * argv[]){
	// TODO	
	return 0;		
}

int is_built_in_command(char * binary){
	if(strncmp("exit", binary, strlen(binary)) == 0 || strncmp("jobs", binary, strlen(binary)) == 0){
		// Ensure this isn't an empty string.
		if(strlen(binary) > 0){
			return 1;	
		}
	}
	return 0;
}







