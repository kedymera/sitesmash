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

    // to play around with it
    size_t i;
    while (1) {
        printf("enter index: ");
        scanf("%lu", &i);
        if (i >= numbms) break;
        printf("bms[%lu] = {url:%s, count:%d}\n", i, bms[i].url, bms[i].count);
    }

    free_bms(bms, numbms);
    return 0;
}
