#include <stdio.h>
#include <string.h>
#include "dbg.h"
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

    check(init_logfind(search_mode, &argv[argv_shift]) == 0, 
          "Cannot initialize logfind");

    logfind_start();

    print_results();

    logfind_end();

    return 0;

error:
    return -1;
}