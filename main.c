#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbg.h"

enum SearchMode {
    AND_MODE, OR_MODE
};

char *files[] = {
    "testfile",
    "testfile2"
};

char **results;
int result_count = 0;

char *read_line(FILE *fp) {
    int i = 0;
    char ch;
    size_t buff_len = 0;

    char *buffer = malloc(buff_len + 1);
    check(buffer, "Out of memory.");

    while((ch = fgetc(fp)) != '\n' && ch != EOF) {
        buff_len++;
        void *tmp = realloc(buffer, buff_len + 1);
        check(tmp, "Out of memory.");

        buffer = tmp;
        buffer[i] = (char) ch;
        i++;
    }
    buffer[i] = '\0';

    if (ch == EOF && (i == 0 || ferror(fp))) {
        free(buffer);
        buffer = NULL;
    }

    return buffer;

error:
    if (buffer) free(buffer);
    return NULL;
}

void add_to_results(char *file_name) {
    size_t len = strlen(file_name);

    void *tmp = realloc(results, result_count + 1);
    results = tmp;

    results[result_count] = malloc(len);

    strcpy(results[result_count++], file_name);
}

void free_results() {
    int i = 0;
    while(results[i]) {
        if (results[i]) free(results[i]);
        i++;
    }

    free(results);
}

int process_file_or_mode(FILE *fp, char **keywords) {
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
    }

    return match;
}

char **stradup(char **strarray) {
    int i = 0;
    char **dup = malloc(0);
    check(dup, "Something went wrong.");

    while(strarray[i]) {
        char **tmp = realloc(dup, i + 1);
        dup = tmp;
        dup[i] = malloc(strlen(strarray[i]) + 1);

        strcpy(dup[i], strarray[i]);
        i++;
    }

    return dup;

error:
    return NULL;
}

int process_file_and_mode(FILE *fp, char **keywords) {
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
    }

    int match = 1;
    for(i = 0; i < count; i++) {
        if (match_array[i] == 0) match = 0;
    }

    return match;
}

void print_results() {
    int i;
    for(i = 0; i < result_count; i++) {
        printf("%s\n", results[i]);
    }
}

int main(int argc, char *argv[]) {
    check(argc >= 2, "Please provide more parametes.");
    int argv_shift = 1;

    int search_mode = strcmp(argv[1], "-o") == 0;

    if (search_mode == OR_MODE) {
        check(argc >= 3, "Please provide more parameters.");
        argv_shift = 2;
    }

    char **keywords = &argv[argv_shift];

    results = malloc(0);

    int i;
    for(i = 0; i < 2; i++) {
        char *file_name = files[i];
        FILE *fp = fopen(file_name, "r");
        check(fp, "Can't open a file: %s", file_name);

        int match = 0;

        switch(search_mode) {
            case OR_MODE:
                match = process_file_or_mode(fp, keywords);
                break;
            case AND_MODE:
                match = process_file_and_mode(fp, keywords);
                break;
            default:
                sentinel("Something went wrong");
        }

        if (match) {
            add_to_results(file_name);
        }

        fclose(fp);
    }

    print_results();
    free_results();

    return 0;

error:
    return -1;
}