#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "pipe_reader.h"
#include "monitor.h"
#define xDEBUG 1

/* Read characters from the pipe and echo them to stdout. */
pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;
bool cv_clear_to_write = false;
pthread_cond_t cv;

bool 
lock_write_channel()
{

	pthread_mutex_lock(&write_mutex);	
     while (! cv_clear_to_write ) {	
              pthread_cond_wait(&cv, &write_mutex);	
	}	
	pthread_mutex_unlock(&write_mutex);
	return true;
}

struct Args{
	int	f;
};

void 
write_line(char* buf_ptr, int count){
	printf("ZZZZ YYYY %.*s", count, buf_ptr);
}

void 
write_out_line(char* buf_ptr, int count){
	printf("STDOUT_LINE : %.*s", count, buf_ptr);
}
void 
write_err_line(char* buf_ptr, int count){
	printf("STDERR_LINE : %.*s", count, buf_ptr);
}


#define XXX 1
#ifdef XXX

void
read_lines_from_pipe (int file)
{
	// char buffer[200];
	char* 	buf_ptr = NULL;
	size_t	count = 0;
	FILE 	*stream;
	int 	c;
	
	stream = fdopen (file, "r");
	while ((c = getline(&buf_ptr, &count, stream)) != EOF){
		write_line(buf_ptr, count);
		// putchar (c);
	}
	fclose (stream);
}


void
read_chars_from_pipe (int file)
{
  FILE *stream;
  int c;
  stream = fdopen (file, "r");
  while ((c = fgetc (stream)) != EOF)
    putchar (c);
  fclose (stream);
}
#endif

/* Write some random text to the pipe. */

void
write_to_pipe (char* msg, int file)
{
  FILE *stream;
  stream = fdopen (file, "w");
  fprintf (stream, "%s hello, world!\n", msg);
  fprintf (stream, "%s goodbye, world!\n", msg);
  fclose (stream);
}

void* stdout_thread(void* arg)
{
	pipe_reader_t r = (pipe_reader_t) arg;
#ifdef DEGUG
	printf("pipe_reader stdout_thread \n");
	pipe_reader_dump(r);
#endif

	pipe_reader_read_lines(r);
	// r->read_lines_f(r);

#ifdef DEGUG
	printf("stdout_thread \n");
#endif
	return NULL;
}
void* stderr_thread(void* arg)
{
	pipe_reader_t r = (pipe_reader_t) arg;
#ifdef DEGUG
	printf("pipe_reader stderr_thread \n");
	pipe_reader_dump(r);
#endif
	pipe_reader_read_lines(r);
	// r->read_lines_f(r);

#ifdef DEGUG
	printf("stderr_thread \n");
#endif
	return NULL;
}

int
main (void)
{
	pid_t pid;
	int stdout_pipe[2];
	int stderr_pipe[2];

	/* Create the pipe. */
	if (pipe (stdout_pipe)) {
		fprintf (stderr, "Pipe failed.\n");
		return EXIT_FAILURE;
    }
	/* Create the pipe. */
	if (pipe (stderr_pipe)) {
		fprintf (stderr, "Pipe failed.\n");
		return EXIT_FAILURE;
    }
	/* Create the child process. */
	pid = fork ();
	if (pid < (pid_t) 0) {
		/*
		**  The fork failed. 
		*/
		fprintf (stderr, "Fork failed.\n");
		return EXIT_FAILURE;
	} else 	if (pid == (pid_t) 0) {
		/* 
		** setup the child process to write to the write end of the two pipes
		** Close read ends of those pipes first. 
		*/
		close(stdout_pipe[0]);
		close(stderr_pipe[0]);
		/**
		* now attach the pipes to stdout and stderr respectively
		*/
		// int st1 = dup2(STDOUT_FILENO, stdout_pipe[1]);
		// int st2 = dup2(STDERR_FILENO, stderr_pipe[1]);
		// if( st1 != 0 || st2 != 0)
		// 	printf("dup2 %d %d \n", st1, st2);
		/**
		*
		*/
		// write_to_pipe ("stdout: ", STDOUT_FILENO);
		// write_to_pipe ("stderr: ", STDERR_FILENO);

		write_to_pipe ("stdout: ", stdout_pipe[1]);
		write_to_pipe ("stderr: ", stderr_pipe[1]);
		// close (stdout_pipe[1]);
		// read_from_pipe (stdout_pipe[0]);
		return EXIT_SUCCESS;
	} else  {
		/* 
		** This is the parent process. Set up eader threads to read from 
		** the child processes via the read end of both stdout_pipe and stderr_pipe
		** Close write end of these two pipes first. 
		*/
		close (stdout_pipe[1]); close (stderr_pipe[1]);
#define MONITOR_ON 1		
#ifdef MONITOR_ON		
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
#else
		pipe_reader_t out_reader = pipe_reader_create(stdout_pipe[0], &write_line);
		pipe_reader_t err_reader = pipe_reader_create(stderr_pipe[0], &write_line);
#ifdef DEBUG		
		pipe_reader_dump(out_reader);
		pipe_reader_dump(err_reader);
#endif		
		pthread_cond_init(&cv, NULL);
		pthread_t o_thread;
		pthread_t e_thread;
		int rc1 = pthread_create(&o_thread, NULL, stdout_thread, (void*)out_reader  );
		int rc2 = pthread_create(&e_thread, NULL, stderr_thread, (void*)err_reader  );
		pthread_join(o_thread, NULL);
		pthread_join(e_thread, NULL);
		// read_from_pipe (stdout_pipe[0]);
		// close (stdout_pipe[0]);
		// write_to_pipe (stdout_pipe[1]);
#endif
		return EXIT_SUCCESS;
	}
}