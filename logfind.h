#ifndef __logfind_h
#define __logfind_h

#include <stdio.h>

enum SearchMode {
    AND_MODE, OR_MODE
};

int init_result_table();
void add_to_results(char *file_name);
void free_results();
void print_results();

int process_file(FILE *fp, char **keywords, int search_mode);

#endif