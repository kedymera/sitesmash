#include <stdio.h>
#include <stdlib.h>

struct bm {
    char *url;
    size_t urllen;
    char count;
};

size_t extract_url(FILE *fp, char **s) {
    char c;
    size_t len = 0, sz = 32;
    *s = malloc(sz * sizeof(char));
    while ((c = fgetc(fp)) != '"') {
        ++len;
        if (len >= sz) {
            sz *= 2;
            *s = realloc(*s, sz * sizeof(char));
        }
        (*s)[len-1] = c;
    }
    (*s)[len] = '\0';
    *s = realloc(*s, (len+1) * sizeof(char));
    //printf("adding url:%s which is of length %lu\n", *s, len);
    return len;
}

size_t get_bms(char *filename, struct bm **bms) {
    char c;
    const char *pre = "<A HREF=\"";
    const size_t prelen = 9;
    size_t i = 0, numbms = 0, sz = 32;
    *bms = malloc(sz * sizeof(struct bm));
    FILE *fp = fopen(filename, "r");
    while ((c = fgetc(fp)) != EOF) {
        if (c == pre[i]) {
            ++i;
            if (i >= prelen) {
                ++numbms;
                if (numbms > sz) {
                    sz *= 2;
                    *bms = realloc(*bms, sz * sizeof(struct bm));
                }
                char *s = NULL;
                (*bms)[numbms-1].urllen = extract_url(fp, &s);
                (*bms)[numbms-1].url = s;
                (*bms)[numbms-1].count = 1;
                //printf("adding url:%s which is of length %lu\n", (*bms)[numbms].url, (*bms)[numbms].urllen);
                i = 0;
            }
        } else {
            i = 0;
        }
    }


    fclose(fp);
    return numbms;
}

void print_bms(struct bm *bms, size_t numbms) {
    printf("{\n");
    for (size_t i = 0; i < numbms; ++i) {
        printf("url:%s, count:%d\n", bms[i].url, bms[i].count);
    }
    printf("}\n");
}

void free_bms(struct bm *bms, size_t numbms) {
    for (size_t i = 0; i < numbms; ++i) {
        free(bms[i].url);
    }
    free(bms);
}
