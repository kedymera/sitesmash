#include <stdio.h>
#include "bms.h"

int main() {
    struct bm *bms = NULL;

    char *filename = "bookmarks.html";
    printf("reading bookmarks from file %s\n", filename);
    size_t numbms = get_bms(filename, &bms);
    printf("%lu bms extracted from file\n", numbms);

    printf("trimming protocols from bookmarks\n");
    trim_protocols(bms, numbms);

    free_bms(bms, numbms);

    return 0;
}
