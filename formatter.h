#ifndef FORMATTER_H
#define FORMATTER_H
/*
* This class/object is intended to be a configurable formatter for lines of output
* from a cli command. The intent is to colorize the output lines based on whether
* they are from stdout or stderr.
*
* In addition the formatter can write colorized status or summary after a command completes.
* the color and format of the status line depends on success or failure status == 0
*
* CURRENT STATUS - no config is supported. Formats and colors are hard coded
*/
struct formatter_s;
typedef struct formatter_s *formatter_t;

struct formatter_s{
	int	something;
	char* cmd;
};

formatter_t formatter_create(char* cmd);
void formatter_format_stdout_line(formatter_t fmt, char* buf_ptr, int count);
void formatter_format_stderr_line(formatter_t fmt, char* buf_ptr, int count);
void formatter_format_status_line(formatter_t f, int status);
#endif