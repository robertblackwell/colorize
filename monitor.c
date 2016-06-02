#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "monitor.h"
#include "pipe_reader.h"
#define xDEBUG 1


void* m_thread_func(void* arg)
{
	pipe_reader_t r = (pipe_reader_t) arg;
#ifdef DEGUG
	printf("pipe_reader stdout_thread \n");
	pipe_reader_dump(r);
#endif

	r->read_lines_f(r);

#ifdef DEGUG
	printf("stdout_thread \n");
#endif
	return NULL;
}

void monitor_dump(monitor_t m){
	printf("monitor : %lx read_fd : %d m_reader: %lx m_thread: %lx\n", 
		(long)m, m->read_fd, (long)m->m_reader, (long)m->m_thread);
}
monitor_t monitor_create(int read_fd, monitor_write_func f){
	void* tmp = malloc(sizeof(struct monitor_s));
	// printf("monitor_create tmp: %lx\n", (long)tmp);
	monitor_t m = (monitor_t)tmp;
	m->read_fd = read_fd;
	m->write_f = f;
	return m;
}

void monitor_start(monitor_t m){
	pipe_reader_t m_reader = pipe_reader_create(m->read_fd, m->write_f);
	m->m_reader = m_reader;
#ifdef DEBUG		
	pipe_reader_dump(m_reader);
#endif		
	pthread_t m_thread;
	m->m_thread = m_thread;
	int rc1 = pthread_create(&m_thread, NULL, m_thread_func, (void*)m_reader  );
}

pthread_t monitor_get_thread(monitor_t m){
	return m->m_thread;
}