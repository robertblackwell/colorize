#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#include "pipe_reader.h"

/**
* dump a pipe_reader structure
*/
void pipe_reader_dump(pipe_reader_t pr){
	printf("pipe_reader : %lx fd : %d write_f : %lx read_lines_f : %lx \n", (long)pr, pr->fd, (long)pr->write_f, (long)pr->read_lines_f);
}

/**
* create a pipe_reader structure
*/
pipe_reader_t pipe_reader_create(int fd, pipe_reader_write_func f){
	struct pipe_reader_s *tmp = malloc(sizeof(struct pipe_reader_s));
#ifdef DEBUG	
	printf("pipe_reader_create fd: %d  malloc: %x  write_f: %x\n", fd, (int)tmp, (int)f);
#endif
	tmp->fd 			= fd;
	tmp->write_f 		= f;
	tmp->read_lines_f	= &pipe_reader_read_lines;
	
	return tmp;
}

/**
* pipe_reader_read_lines method - reads lines of text from the readers pipe
* and passes them to the write_f call back
*/
void pipe_reader_read_lines(pipe_reader_t pr)
{
	// char buffer[200];
	char* 	buf_ptr = NULL;
	size_t	count = 0;
	FILE 	*stream;
	int 	c;
	int		fd = pr->fd;
#ifdef DEBUG	
	printf("pipe_reader_read_lines pr: %lx write_f : %lx\n",(long)pr, (long)pr->write_f);
#endif
	stream = fdopen (fd, "r");
	while ((c = getline(&buf_ptr, &count, stream)) != EOF){
		// pipe_reader_write_line(buf_ptr, count);
		pr->write_f(buf_ptr, count);
		// putchar (c);
	}
	fclose (stream);
}


