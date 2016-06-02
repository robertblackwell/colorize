#ifndef MONITOR_H
#define MONITOR_H 1

#include <pthread.h>
#include "pipe_reader.h"

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