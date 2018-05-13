#ifndef __io_utils_h
#define __io_utils_h

#include <stdio.h>

char *read_line(FILE *fp);
size_t file_list(const char *dir_name, char ***ls);

#endif