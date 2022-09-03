#include <stdio.h>
#include <stdlib.h>
#include "bms.h"

int main() {
    struct bm *bms = NULL, *newbms = NULL;

    char *bmsfile = "out/links", *newbmsfile = "bookmarks.html";
    size_t numbms, numnewbms;

    printf("Reading existing links from file %s\n", bmsfile);
    numbms = read_bms(bmsfile, &bms);
    printf("%lu bms extracted\n", numbms);

    printf("Reading bookmarks from file %s\n", newbmsfile);
    numnewbms = get_bms(newbmsfile, &newbms);
    printf("%lu bms extracted\n", numnewbms);

    printf("Trimming protocols from new bookmarks\n");
    trim_protocols(newbms, numnewbms);

    printf("Sorting new bookmarks\n");
    sort_bms(newbms, numnewbms);

    printf("Combine the two arrays\n");
    struct bm *combinedbms = combine_bms(bms, numbms, newbms, numnewbms);
    free(bms);
    free(newbms);

    size_t numcombinedbms = numbms + numnewbms;
    print_bms(combinedbms, numcombinedbms);
    free_bms(combinedbms, numcombinedbms);

    return 0;
}
