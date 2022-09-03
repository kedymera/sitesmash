#include <stdio.h>
#include "bms.h"

int main() {
    struct bm *bms = NULL;
    size_t numbms = get_bms("bookmarks.html", &bms);
    printf("%lu bms extracted from file\n", numbms);

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
