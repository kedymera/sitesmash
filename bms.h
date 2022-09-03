#pragma once
#include <stdio.h>

struct bm {
    char *url;
    size_t urllen;
    char count;
};

/* traverses file until first encountered `"', building a string (allocates) */
size_t extract_url(FILE *fp, char **s);

/* parses file `filename' for all links and creates an array of bms therefrom (allocates) */
size_t get_bms(char *filename, struct bm **bms);

/* print an array of bms (probably for debugging) */
void print_bms(struct bm *bms, size_t numbms);

/* free an allocated array of bms of length numbms */
void free_bms(struct bm *bms, size_t numbms);
