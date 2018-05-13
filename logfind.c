#include "logfind.h"
#include <stdio.h>
#include <stdlib.h>
#include "io_utils.h"
#include "dbg.h"

static char **results;
static int results_count = 0;

int init_result_table() {
    results = malloc(0);
    check_mem(results);

    return 0;
error:
    return -1;
}

void add_to_results(char *file_name) {
    size_t len = strlen(file_name);

    results = realloc(results, sizeof(char*) * (results_count + 1));

    results[results_count] = malloc(len + 1);

    strcpy(results[results_count++], file_name);
}

void free_results() {
    int i;
    for(i = 0; i < results_count; i++) {
        if (results[i]) free(results[i]);
    }

    free(results);
}

void print_results() {
    int i;
    for(i = 0; i < results_count; i++) {
        printf("%s\n", results[i]);
    }
}

static int process_file_or_mode(FILE *fp, char **keywords) {
    int match = 0;
    char *line;
    int i;
    while ((line = read_line(fp)) != NULL) {
        i = 0;

        while(keywords[i]) {
            if (strstr(line, keywords[i])) {
                match = 1;
                break;
            }
            i++;
        }
        free(line);
    }

    return match;
}

static int process_file_and_mode(FILE *fp, char **keywords) {
    char *line;
    int i = 0;

    int *match_array = malloc(0);
    while(keywords[i]) {
        match_array = realloc(match_array, sizeof(int) * (i + 1));
        match_array[i] = 0;
        i++;
    }

    int count = i;

    while ((line = read_line(fp)) != NULL) {
        i = 0;

        while(keywords[i]) {
            if (strstr(line, keywords[i])) {
                match_array[i] = 1;
                break;
            }
            i++;
        }
        free(line);
    }

    int match = 1;
    for(i = 0; i < count; i++) {
        if (match_array[i] == 0) match = 0;
    }

    free(match_array);

    return match;
}

int process_file(FILE *fp, char **keywords, int search_mode) {
    int result;
    if (search_mode == OR_MODE) {
        result = process_file_or_mode(fp, keywords);
    } else {
        result = process_file_and_mode(fp, keywords);
    }
    return result;
}