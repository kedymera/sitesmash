#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "bms.h"

unsigned char parse_def_count(char *arg) {
    char *end;
    errno = 0;
    long x = strtol(arg, &end, 10);
    if (errno != 0 || arg == end) {
        fprintf(stderr, "couldn't parse %s; default to 1\n", arg);
        return 1;
    }
    if (x < 0) return 0;
    if (x > 255) return 255;
    return x;
}

int main(int argc, char *argv[]) {
    unsigned char def_count = argc > 1 ? parse_def_count(argv[1]) : 1;

    struct bm *bms = NULL, *newbms = NULL;
    size_t numbms, numnewbms;
    const char *bmsfile = "out/links", *newbmsfile = "bookmarks.html";

    printf("Reading existing links from file %s\n", bmsfile);
    numbms = read_bms(bmsfile, &bms);
    if (!numbms) {
        return 1;
    }
    printf("%lu bms extracted\n", numbms);

    printf("Reading bookmarks from file %s\n", newbmsfile);
    numnewbms = get_bms(newbmsfile, &newbms, def_count);
    printf("%lu bms extracted\n", numnewbms);
    if (numnewbms) {
        printf("Trimming protocols from new bookmarks\n");
        trim_protocols(newbms, numnewbms);

        printf("Sorting new bookmarks\n");
        sort_bms(newbms, numnewbms);

        printf("Combine the two arrays\n");
        struct bm *tmp = combine_bms(bms, numbms, newbms, numnewbms);
        numbms += numnewbms;

        free(bms);
        free(newbms);

        bms = tmp;
    }

    printf("Resolve duplicates\n");
    numbms = resolve_duplicate_bms(&bms, numbms);

    printf("Writing array of %lu bms to file %s\n", numbms, bmsfile);
    write_bms(bms, numbms, bmsfile);
    free_bms(bms, numbms);

    return 0;
}
