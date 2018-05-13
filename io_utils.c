#include <stdlib.h>
#include <dirent.h>
#include "io_utils.h"
#include "dbg.h"

char *read_line(FILE *fp) {
    int i = 0;
    char ch;
    size_t buff_len = 0;

    char *buffer = malloc(buff_len + 1);
    check_mem(buffer);

    while((ch = fgetc(fp)) != '\n' && ch != EOF) {
        buff_len++;
        void *tmp = realloc(buffer, buff_len + 1);
        check_mem(tmp);

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

size_t file_list(const char *dir_name, char ***ls) {
    int count = 0;
    DIR *dir = NULL;
    struct dirent *ep = NULL;

    dir = opendir(dir_name);
    check(dir, "No such directory: %s", dir_name);

    ep = readdir(dir);
    while(ep != NULL) {
        *ls = realloc(*ls, sizeof(char*) * (count + 1));
        (*ls)[count++] = strdup(ep->d_name);

        ep = readdir(dir);
    }

    closedir(dir);

    return count;
error:
    return -1;
}
