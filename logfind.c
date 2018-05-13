#include "logfind.h"
#include <stdio.h>
#include <stdlib.h>
#include <fnmatch.h>
#include <ftw.h>
#include "io_utils.h"
#include "dbg.h"

static int process_file(const char *fpath, const struct stat *sb, int typeflag);

static char **results;
static int results_count = 0;

static char **keywords;
static int search_mode = AND_MODE;

static char **filters;

static size_t init_filters(char ***__filters) {
    *__filters = realloc(*__filters, 0);

    FILE *fp = fopen(".logfind", "r"); 
    if (fp == NULL) {
        *__filters = malloc(sizeof(char*));
        *__filters[0] = strdup("*");
        return 1;
    }

    int count = 0;
    char *line;
    while ((line = read_line(fp)) != NULL) {
        *__filters = realloc(*__filters, sizeof(char*) * (count + 1));
        (*__filters)[count++] = strdup(line);
    }

    return count;
}

int init_logfind(int __search_mode, char **__keywords) {
    search_mode = __search_mode;
    keywords = __keywords;

    results = malloc(0);
    check_mem(results);

    init_filters(&filters);

    return 0;
error:
    return -1;
}

int logfind_start() {
    ftw(".", process_file, 20);

    return 0;
}

void add_to_results(char *file_name) {
    size_t len = strlen(file_name);

    results = realloc(results, sizeof(char*) * (results_count + 1));

    results[results_count] = malloc(len + 1);

    strcpy(results[results_count++], file_name);
}

int logfind_end() {
    int i;
    for(i = 0; i < results_count; i++) {
        if (results[i]) free(results[i]);
    }

    free(results);

    i = 0;
    while(filters[i]) {
        if (filters[i]) free(filters[i++]);
    }
    free(filters);

    return 0;
}

void print_results() {
    int i;
    for(i = 0; i < results_count; i++) {
        printf("%s\n", results[i]);
    }
}

static int process_file_or_mode(FILE *fp) {
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

static int process_file_and_mode(FILE *fp) {
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

static int process_file(const char *fpath, const struct stat *sb, int typeflag) {
    FILE *fp = fopen(fpath, "r");

    int is_match_filters = 0;
    int i = 0;
    while(filters[i]) {
        if (fnmatch(filters[i++], fpath, 0) == 0) {
            is_match_filters = 1;
            break;
        }
    }

    if (!is_match_filters) return 0;

    int result;
    if (search_mode == OR_MODE) {
        result = process_file_or_mode(fp);
    } else {
        result = process_file_and_mode(fp);
    }
    if (result) add_to_results((char*)fpath);

    fclose(fp);

    return 0;
}