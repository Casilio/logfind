#ifndef __logfind_h
#define __logfind_h

#include <stdio.h>
#include <ftw.h>

enum SearchMode {
    AND_MODE, OR_MODE
};

int init_logfind(int search_mode, char **keywords);
void add_to_results(char *file_name);
void print_results();
int logfind_start();
int logfind_end();

static int process_file(const char *fpath, const struct stat *sb, int typeflag);

#endif