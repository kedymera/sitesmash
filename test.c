#include <stdio.h>
#include "bms.h"

int main() {
    struct bm *bms = NULL;

    char *filename = "bookmarks.html";
    printf("reading bookmarks from file %s\n", filename);
    size_t numbms = get_bms(filename, &bms, 1);
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

    char *outfile = "out/links";
    printf("writing %lu bms to file %s\n", numbms, outfile);
    write_bms(bms, numbms, outfile);

    free_bms(bms, numbms);

    char *infile = "out/links2";
    printf("reading bms from file %s\n", infile);
    numbms = read_bms(infile, &bms);
    printf("%lu bms extracted from file\n", numbms);

    // to play around with it
    while (1) {
        printf("enter index: ");
        scanf("%lu", &i);
        if (i >= numbms) break;
        printf("bms[%lu] = {url:%s, count:%d}\n", i, bms[i].url, bms[i].count);
    }
    free_bms(bms, numbms);

    return 0;
}
