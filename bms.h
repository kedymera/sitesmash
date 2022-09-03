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

/* trim the protocols off all bms in bm array
 * i.e. https://non.sen.se -> non.sen.se
 * assumes all urls have one */
void trim_protocols(struct bm *bms, size_t numbms);

/* trim the protocol off a string containing a url
 * i.e. https://non.sen.se -> non.sen.se
 * assumes all urls have one */
void trim_protocol(char **s, size_t *len);
