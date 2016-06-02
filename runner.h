#ifndef RUNNER_H
#define RUNNER_H 1

/*
* An instance of this class takes a cmd string (like a execvp 'file' argument), an argc, argv like in
* execvp, redirects stdout and stderr to the given pipe ends and then runs the command.
*
* Assumes something else is reading the two pipe ends to get the command output
*/

struct runner_s;
typedef struct runner_s *runner_t;
struct runner_s{
	int		out_fd;
	int		err_fd;
	char*	cmd;
	int		argc;
	char**	argv;
};

runner_t runner_create(char* cmd, int argc, char* argv[], int out_fd, int err_fd);
int	runner_run(runner_t r);
void runner_dump(runner_t r);
#endif