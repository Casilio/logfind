#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbg.h"
#include "io_utils.h"
#include "logfind.h"

int argv_shift, search_mode;

int is_valid_params(int argc, char **argv) {
    if (argc < 2) return -1;
    argv_shift = 1;

    search_mode = strcmp(argv[1], "-o") == 0;

    if (search_mode == OR_MODE) {
        if (argc < 3) return -1;
        argv_shift = 2;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int rc = is_valid_params(argc, argv);
    check(rc == 0, "Invalid params.");

    char **files = malloc(0);
    int list_count = file_list(".", &files);

    char **keywords = &argv[argv_shift];

    rc = init_result_table();
    check(rc == 0, "Cannot initialize result table.");

    int i;
    for(i = 0; i < list_count; i++) {
        char *file_name = files[i];
        FILE *fp = fopen(file_name, "r");
        check(fp, "Can't open a file: %s", file_name);

        int match = process_file(fp, keywords, search_mode);

        if (match) {
            add_to_results(file_name);
        }

        fclose(fp);
    }

    for(i = 0; i < list_count; i++) {
        free(files[i]);
    }
    free(files);

    print_results();
    free_results();

    return 0;

error:
    return -1;
}