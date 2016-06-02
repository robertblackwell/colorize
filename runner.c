#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "runner.h"

runner_t runner_create(char* cmd, int argc, char* argv[], int fd_out, int fd_err)
{
	void* tmp = malloc(sizeof(struct runner_s));
	runner_t r = (runner_t)tmp;
	r->cmd = cmd;
	r->argc = argc;
	r->argv = &argv[0];
	r->out_fd = fd_out;
	r->err_fd = fd_err;
	
	int st1 = 0;
	int st2 = 0;
	if( r->out_fd > 0 )
		st1 = dup2(r->out_fd, STDOUT_FILENO);
	if( r->err_fd > 0 )
		st2 = dup2(r->err_fd, STDERR_FILENO);
	if( st1 < 0 || st2 < 0)
		printf("dup2 %d %d \n", st1, st2);
	
	return r;
}

void runner_dump(runner_t r){
	printf("runner : %lx cmd: %s argc : %d  argv[1] %s  fd_out : %d fd_err : %d \n",
			(long)r, r->cmd, r->argc, r->argv[1], r->out_fd, r->err_fd);
			
	for(int i = 0; i < r->argc; i++){
		printf("argv[%d] = %s \n", i, r->argv[i]); 
	}		
}

int runner_run(runner_t r)
{
	printf("runner_run \n");
	char* a[] = {"-al", NULL};
	// execvp("ls", a);
	
	execvp(r->cmd, r->argv);
	return 0;
}