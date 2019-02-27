#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <zconf.h>
#include <unistd.h>
#include <stdbool.h>

#define SPACE_TOKEN			' '
#define TAB_TOKEN			'\t'
#define TERMINAL_TOKEN		'\0'
#define NEWLINE_TOKEN      	'\n'
#define STRLEN_MAX			4096
#define PRINT_ERROR_AND_EXIT \
    fprintf(stdout, "[Error]\n"); \
    return 0; \


char*
clear_and_analyze_input(char**, size_t);

int main() {
	char* strbuf = (char*) malloc(sizeof(char) * STRLEN_MAX);
	ssize_t bytes_read_s;
	size_t bytes_read;
	
	bytes_read_s = read(STDIN_FILENO, strbuf, STRLEN_MAX);
	
	printf("Read %ld bytes\n", bytes_read_s);
	
	if (bytes_read_s > 0 && bytes_read_s < STRLEN_MAX) {
		strbuf = realloc(strbuf, (size_t) bytes_read_s);
		bytes_read = (size_t) bytes_read_s;
	} else {
		PRINT_ERROR_AND_EXIT
	}
	
	strbuf = clear_and_analyze_input(&strbuf, bytes_read);
	
	if (!strbuf) {
		PRINT_ERROR_AND_EXIT
	}
	
	printf("%s", strbuf);
	
	return 0;
}

char*
clear_and_analyze_input(char** ps, size_t bytes_read) {
	char c;
	size_t i, shift = 0;
	int cp_count = 0, sp_count = 0;
	
	for (i = 0; i < bytes_read; i++) {
		c = (*ps)[i];
		
		if (c == SPACE_TOKEN || c == TAB_TOKEN || c == NEWLINE_TOKEN) {
			shift++;
		} else {
			switch (c) {
				case ')':
					if (!cp_count) {
						return NULL;
					} else {
						cp_count--;
					}
					break;
				case '(':
					cp_count++;
					break;
				case ']':
					if (!sp_count) {
						return NULL;
					} else {
						sp_count--;
					}
					break;
				case '[':
					sp_count++;
					break;
				default:
					break;
			}
			
			(*ps)[i - shift] = c;
		}
	}
	
	if (sp_count || cp_count) {
		return NULL;
	}
	
	(*ps)[bytes_read - shift] = TERMINAL_TOKEN;
	
	return realloc(*ps, (size_t) bytes_read - shift);
}