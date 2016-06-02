#include <stdio.h>
#include "color.h"

char* color_text(char* text, color_t c){

	char* new_text;
	int count = asprintf(&new_text, "%s%s%s", c, text, ANSI_COLOR_RESET);
	return new_text;
	
}
