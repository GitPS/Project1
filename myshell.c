/*
 * Josh Hursey
 * Feb. 11, 2013
 *
 * CS441/541: Project 1 Part 1
 *
 */
#include "myshell.h"

int main(int argc, char * argv[]) {

    /*
     * Below is just an example of how to use the support functionality.
     * You will want to remove this code when you get started, but
     * it may serve as a helpful reference while you are getting started.
     */
    char command[1024] = "echo Hello CS441 ; echo Welcome to my shell ; ls & sleep 10 &";
    int num_jobs = 0;
    job_t *loc_jobs = NULL;
    int i, j;

    /*
     * Allocate some space for the job_t array
     * Necessary so split_input_into_jobs() function has a memory
     * handle to work with.
     */
    loc_jobs = (job_t*)malloc(sizeof(job_t) * 1);
    if( NULL == loc_jobs ) {
        fprintf(stderr, "Error: Failed to allocate memory! Critical failure on %d!", __LINE__);
        exit(-1);
    }

    /*
     * Split out the individual jobs from the larger command
     */
    printf("Splitting the command: <%s>\n", command);
    split_input_into_jobs(command, &num_jobs, &loc_jobs);

    /*
     * For each of those jobs split out the argument set
     */
    for( i = 0; i < num_jobs; ++i ) {
        printf("Processing the job: <%s>\n", loc_jobs[i].full_command);

        split_job_into_args( &(loc_jobs[i]) );

        /*
         * Display the arguments
         */
        printf("Arguments: ");
        fflush(NULL);
        for( j = 0; j < loc_jobs[i].argc; ++j ) {
            printf(" [%s]", loc_jobs[i].argv[j]);
            fflush(NULL);
        }
        printf("\n");
        fflush(NULL);
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

