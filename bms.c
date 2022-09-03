#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bms.h"

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

void trim_protocols(struct bm *bms, size_t numbms) {
    size_t i;
    for (i = 0; i < numbms; ++i)
        trim_protocol(&bms[i].url, &bms[i].urllen);
    return;
}

void trim_protocol(char **s, size_t *len) {
    size_t i, j = 0;
    for (i = 0; i < *len; ++i) {
        if ((*s)[i] == "://"[j]) {
            ++j;
            if (j == 3) {
                break;
            }
        }
    }
    *len -= i+1; 
    for (j = 0; j < *len; ++j) {
        (*s)[j] = (*s)[j+i+1];
    }
    (*s)[j] = '\0';
    *s = realloc(*s, (*len+1) * sizeof(char));
    return;
}

void write_bms(struct bm *bms, size_t numbms, char *filename) {
    FILE *fp = fopen(filename, "w");
    size_t i;
    for (i = 0; i < numbms; ++i)
        fprintf(fp, "%s %d\n", bms[i].url, bms[i].count);

    fclose(fp);
    return;
}

size_t read_bms(char *filename, struct bm **bms) {
    char c;
    size_t numbms = 0, bmssz = 32, buffsz = 32, i = 0;
    *bms = malloc(bmssz * sizeof(struct bm));
    char *tmp = malloc(buffsz * sizeof(char));
    FILE *fp = fopen(filename, "r");
    while ((c = fgetc(fp)) != EOF) {
        if (i >= buffsz) {
            buffsz *= 2;
            tmp = realloc(tmp, buffsz);
        }
        switch (c) {
            case ' ':
                tmp[i++] = '\0';

                (*bms)[numbms].url = malloc(i * sizeof(char));
                strncpy((*bms)[numbms].url, tmp, i);

                i = 0;
                break;
            case '\n':
                tmp[i++] = '\0';

                (*bms)[numbms].count = atoi(tmp);

                i = 0;
                ++numbms;
                printf("numbms is %lu and bmssz is %lu\n", numbms, bmssz);
                if (numbms >= bmssz) {
                    printf("reallocing :)\n");
                    bmssz *= 2;
                    *bms = realloc(*bms, bmssz * sizeof(struct bm));
                }
                break;
            default:
                tmp[i++] = c;
        }
    }
    free(tmp);


    fclose(fp);
    return numbms;
}
