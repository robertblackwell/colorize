#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <getopt.h>
#include "pipe_reader.h"
#include "monitor.h"
#include "runner.h"
#include "color.h"
#include "formatter.h"

#define xDEBUG 1

formatter_t formatter;

/*
* These two functions wrap the formatters output function so that the
* formatter_t instance can be passed in. Should really do this by allowing
* a conetext pointer to go along with the write callback on the monitor_t
* creation call. 
*/
void write_out_line(char* buf_ptr, int count){
	formatter_format_stdout_line(formatter, buf_ptr, count);
}
void write_err_line(char* buf_ptr, int count){
	formatter_format_stderr_line(formatter, buf_ptr, count);
}
void usage(char* cmd){
	printf("Usage: %s [options] cmd arg1 arg2 .... argn\n", cmd);
	printf("\tExecutes cmd with given arguments, colorizing stdout and stderr\n");
	printf("\tand providing a colorized summary/status line\n");
	printf("\n\tOptions\n");
	printf("\n\t-p\tThere are no options currently\n");
}

int main(int argc, char* argv[])
{
	int opt;
	int nsecs;
	while ((opt = getopt(argc, argv, "hnt:")) != -1) {
        switch (opt) {
        case 'n':
			printf("n - \n");
            break;
		case 'h' :
			usage(argv[0]);
			exit(EXIT_SUCCESS);
            break;
        case 't':
            nsecs = atoi(optarg);
			printf("t %s\n", optarg);
            break;
        default: /* '?' */
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }
	pid_t pid;
	/* Create 2 pipes. */
	int stdout_pipe[2], stderr_pipe[2];
	if (pipe (stdout_pipe)) {fprintf (stderr, "Pipe failed.\n"); return EXIT_FAILURE;}
	if (pipe (stderr_pipe)) {fprintf (stderr, "Pipe failed.\n");	return EXIT_FAILURE;}

	formatter = formatter_create(argv[1]);

	pid = fork ();
	if (pid < (pid_t) 0) {
		fprintf (stderr, "Fork failed.\n"); return EXIT_FAILURE;
	} else 	if (pid == (pid_t) 0) {
		/* 
		** setup the child process to write to the write end of the two pipes
		** Close read ends of those pipes first. 
		*/
		close(stdout_pipe[0]);close(stderr_pipe[0]);
		
		char* 	_argv[100];
		int		_argc = argc-1;
		for(int i = 1; i < argc; i++){
			_argv[i-1] = argv[i];
		}
		_argv[_argc] = NULL;
		
		runner_t r = runner_create(argv[1], _argc, _argv, stdout_pipe[1], stderr_pipe[1]);
#ifdef DEBUG
		runner_dump(r);
#endif
		runner_run(r);
		return EXIT_SUCCESS;
	} else  {
		/* 
		** This is the parent process. Set up 2 monitors (with threads inside) to read from 
		** the child processes via the read end of both stdout_pipe and stderr_pipe
		** Close write end of these two pipes first. 
		*/
		close (stdout_pipe[1]); close (stderr_pipe[1]);

		monitor_t out_monitor = monitor_create(stdout_pipe[0], &write_out_line);
		monitor_t err_monitor = monitor_create(stderr_pipe[0], &write_err_line);
#ifdef DEBUG		
		monitor_dump(out_monitor);
		monitor_dump(err_monitor);
#endif		
		monitor_start(out_monitor);
		monitor_start(err_monitor);
		pthread_join(monitor_get_thread(out_monitor), NULL);
		pthread_join(monitor_get_thread(err_monitor), NULL);
		int child_status;
		wait(&child_status);
		formatter_format_status_line(formatter, child_status);
		return child_status;	
	}
}