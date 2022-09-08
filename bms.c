#include <stddef.h>
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

size_t get_bms(const char *filename, struct bm **bms, unsigned char def_count) {
    char c;
    const char *pre = "<A HREF=\"";
    const size_t prelen = 9;
    size_t i = 0, numbms = 0, sz = 32;
    *bms = malloc(sz * sizeof(struct bm));
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "File not found: %s\n", filename);
        free(*bms);
        *bms = NULL;
        return 0;
    }
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
                (*bms)[numbms-1].count = def_count;
                (*bms)[numbms-1].visitedthissession = false;
                //printf("adding url:%s which is of length %lu\n", (*bms)[numbms].url, (*bms)[numbms].urllen);
                i = 0;
            }
        } else {
            i = 0;
        }
    }
    *bms = realloc(*bms, numbms * sizeof(struct bm));

    fclose(fp);
    return numbms;
}

void print_bms(struct bm *bms, size_t numbms) {
    printf("{\n");
    for (size_t i = 0; i < numbms; ++i) {
        printf("url:%s, count:%d (visitedthissession:%d)\n", bms[i].url, bms[i].count, bms[i].visitedthissession);
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

void write_bms(struct bm *bms, size_t numbms, const char *filename) {
    FILE *fp = fopen(filename, "w");
    size_t i;
    for (i = 0; i < numbms; ++i)
        fprintf(fp, "%s %d\n", bms[i].url, bms[i].count);

    fclose(fp);
    return;
}

size_t read_bms(const char *filename, struct bm **bms) {
    char c;
    size_t numbms = 0, bmssz = 32, buffsz = 32, i = 0;
    *bms = malloc(bmssz * sizeof(struct bm));
    char *tmp = malloc(buffsz * sizeof(char));
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "File not found: %s\n", filename);
        free(*bms);
        free(tmp);
        *bms = NULL;
        return 0;
    }
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
                (*bms)[numbms].visitedthissession = false;

                i = 0;
                ++numbms;
                if (numbms >= bmssz) {
                    bmssz *= 2;
                    *bms = realloc(*bms, bmssz * sizeof(struct bm));
                }
                break;
            default:
                tmp[i++] = c;
        }
    }
    free(tmp);
    *bms = realloc(*bms, numbms * sizeof(struct bm));

    fclose(fp);
    return numbms;
}

int compare_bms(const void *bm1, const void *bm2) {
    // struct bm *bm1, struct bm *bm2
    return strcmp(
        ((struct bm *) bm1)[0].url,
        ((struct bm *) bm2)[0].url
    );
}

void sort_bms(struct bm *bms, size_t numbms) {
    qsort(bms, numbms, sizeof(struct bm), compare_bms);
    return;
}

struct bm *combine_bms(struct bm *bms1, size_t numbms1, struct bm *bms2, size_t numbms2) {
    size_t numbms = numbms1 + numbms2;
    struct bm *bms = malloc(numbms * sizeof(struct bm));
    if (numbms1 == 0) {
        memcpy(bms, bms2, numbms2 * sizeof(struct bm));
        return bms;
    } else if (numbms2 == 0) {
        memcpy(bms, bms1, numbms1 * sizeof(struct bm));
        return bms;
    }

    size_t i1 = 0, i2 = 0;

    while (i1+i2 < numbms) {
        if (compare_bms(bms1+i1, bms2+i2) < 0) {
            bms[i1+i2] = bms1[i1];
            ++i1;
            if (i1 >= numbms1) {
                // depleted bms1, so just stick on the rest of bms2
                for (; i1+i2 < numbms; ++i2)
                    bms[i1+i2] = bms2[i2];
            }
        } else {
            bms[i1+i2] = bms2[i2];
            ++i2;
            if (i2 >= numbms2) {
                // depleted bms2, so just stick on the rest of bms1
                for (; i1+i2 < numbms; ++i1)
                    bms[i1+i2] = bms1[i1];
            }
        }
    }

    return bms;
}

char combine_counts(char count1, char count2) {
    if (!count1 || !count2) return 0;
    return count1 > count2 ? count1 : count2;
}

size_t resolve_duplicate_bms(struct bm **bms, size_t numbms) {
    if (numbms < 2) return numbms;
    size_t i = 0, j;
    // bms is assumed sorted, so can just check each bm against the previous
    for (j = 1; j < numbms; ++j) {
        if (compare_bms(*bms+i, *bms+j) == 0) {
            (*bms)[i].count = combine_counts((*bms)[i].count, (*bms)[j].count);
            free((*bms)[j].url);
        } else {
            ++i;
            (*bms)[i] = (*bms)[j];
        }
    }
    *bms = realloc(*bms, ++i * sizeof(struct bm));
    return i;
}
