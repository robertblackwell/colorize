#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "color.h"
#include "formatter.h"

formatter_t formatter_create(char* cmd){
	void* tmp = malloc(sizeof(struct formatter_s));
	formatter_t t = (formatter_t) tmp;
	t->cmd = cmd;
	return t;
}
void formatter_format_stdout_line(formatter_t fmt, char* buf_ptr, int count)
{
	char* t;
	int c = asprintf(&t, "%.*s", count, buf_ptr);
	char* t2 = color_text(t, ANSI_COLOR_GREEN);
	printf("%s", t2);
	// printf("WRITE TO OUTPUT STDOUT_LINE : %.*s", count, buf_ptr);
}
void formatter_format_stderr_line(formatter_t fmt, char* buf_ptr, int count)
{
	char* t;
	int c;
	c = asprintf(&t, "%.*s", count, buf_ptr);
	char* t2 = color_text(t, ANSI_COLOR_RED);
	printf("%s", t2);
}
void formatter_format_status_line(formatter_t fmt, int status)
{
	printf("child process status code : %d \n", status);
	if( status == 0 ){
		char* msg;
		char* txt1 = color_text(fmt->cmd, ANSI_COLOR_CYAN);
		char* txt2 = color_text("completed OK", ANSI_COLOR_BOLD_GREEN);
		int cnt = asprintf(&msg, "%s %s\n", txt1, txt2);
		// printf("%s", msg);
		printf("%sthe command %s %s %s%s completed %s OK %s", 
			ANSI_COLOR_CYAN, 
			ANSI_COLOR_BOLD_GREEN, 
			fmt->cmd,
			// argv[1],
			ANSI_COLOR_RESET,
			ANSI_COLOR_CYAN, 
			ANSI_COLOR_BOLD_GREEN, 
			ANSI_COLOR_RESET
		);
		// write_out_line(msg, cnt);
	}else{
		char* msg;
		char* txt1 = color_text(fmt->cmd, ANSI_COLOR_MAGENTA);
		char* txt2 = color_text("FAILED", ANSI_COLOR_BOLD_RED);
		int cnt = asprintf(&msg, "%s %s\n", txt1, txt2);
		// printf("%s", msg);
		// write_out_line(msg, cnt);
		printf("%sthe command %s %s %s%s %s FAILED %s", 
			ANSI_COLOR_CYAN, 
			ANSI_COLOR_BOLD_GREEN,
			fmt->cmd, 
			// argv[1], 
			ANSI_COLOR_RESET,
			ANSI_COLOR_CYAN, 
			ANSI_COLOR_BOLD_RED, 
			ANSI_COLOR_RESET
		);
	}
}

