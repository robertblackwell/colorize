#ifndef MONITOR_H
#define MONITOR_H 1

#include <pthread.h>
#include "pipe_reader.h"
/*
* This class monitors the read end of a pipe. 
* It creates a pthread which reads lines from that pipe and passes
* those lines onto a function -- write_f 
*
* The reading from the pipe is accomplised via an instance of
* the pipe_reader class.
*
* The primary function of this class is the firing up of the pthread
*/


struct monitor_s;
typedef struct monitor_s *monitor_t;
typedef void (*monitor_write_func)(char* buf_ptr, int count);
	
struct monitor_s{
	int					read_fd;
	monitor_write_func	write_f;
	pipe_reader_t		m_reader;
	pthread_t			m_thread;
};

monitor_t monitor_create(int read_fd, monitor_write_func f);
void monitor_start(monitor_t m);
void monitor_dump(monitor_t m);
pthread_t monitor_get_thread(monitor_t m);
#endif