#include <stdio.h>
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

    free_bms(bms, numbms);

    return 0;
}
