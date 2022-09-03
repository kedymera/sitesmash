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
size_t get_bms(const char *filename, struct bm **bms);

/* print an array of bms (probably for debugging) */
void print_bms(struct bm *bms, size_t numbms);

/* free an allocated array of bms of length numbms, including all used strings */
void free_bms(struct bm *bms, size_t numbms);

/* trim the protocols off all bms in bm array
 * i.e. https://non.sen.se -> non.sen.se
 * assumes all urls have one */
void trim_protocols(struct bm *bms, size_t numbms);

/* trim the protocol off a string containing a url
 * i.e. https://non.sen.se -> non.sen.se
 * assumes all urls have one */
void trim_protocol(char **s, size_t *len);

/* writes bms array to file in format ("%s %d\n", url, count)
 * overwrites file if already exists (mode "w") */
void write_bms(struct bm *bms, size_t numbms, const char *filename);

/* reads bms from bm file of format ("%s %d\n", url, count)
 * i.e. as written by write_bms
 * returns the number of bms read and allocates to pointer *bms provided */
size_t read_bms(const char *filename, struct bm **bms);

/* sorts the bms array given, asciibetically by url */
void sort_bms(struct bm *bms, size_t numbms);

/* combines two bms arrays into one, allocates and returns combined array
 * (free the input arrays if you don't want them anymore,
 * but be careful; the strings are reused, so free with normal free(bms), NOT free_bms(bms)) */
struct bm *combine_bms(struct bm *bms1, size_t numbms1, struct bm *bms2, size_t numbms2);

/* resolves duplicate bms in bms array;
 * if multiple bms have same url, these are condensed into one bm
 * whose count is {0 if any are 0; or max among them if all non-0} */
size_t resolve_duplicate_bms(struct bm **bms, size_t numbms);
