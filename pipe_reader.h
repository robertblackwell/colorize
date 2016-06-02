#ifndef PIPE_READER_H
#define PIPE_READER_H

#include <pthread.h>
#include "pipe_reader.h"

/*
* This class reads lines from a pipe and passes thse lines onto 
* a callback function named -- write_f
*/

struct pipe_reader_s;
typedef struct pipe_reader_s *pipe_reader_t;
typedef void (*pipe_reader_write_func)(char* buf_ptr, int count);
typedef void (read_lines_func)(pipe_reader_t pr);

struct pipe_reader_s{
	int						fd;
	pipe_reader_write_func 	write_f;
	read_lines_func			*read_lines_f; 
	pipe_reader_t			m_reader;
	pthread_t				m_thread;
};


pipe_reader_t pipe_reader_create(int fd, pipe_reader_write_func f);

void pipe_reader_read_lines(pipe_reader_t);
void pipe_reader_read_chars(pipe_reader_t);
void pipe_reader_dump(pipe_reader_t);

#endif