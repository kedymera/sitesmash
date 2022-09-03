#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bms.h"

int main() {
    // TODO: put browser and profile into some config file
    const char *bmsfile = "out/links",
         *browser = "firefox",
         *profile = "hmm";
    char query[1024], command[1024], action;

    printf("Loading bms from file %s\n", bmsfile);
    struct bm *bms;
    size_t numbms = read_bms(bmsfile, &bms);
    sort_bms(bms, numbms);
    numbms = resolve_duplicate_bms(&bms, numbms);

    // TODO: this is literally disgusting
    while (1) {
        printf("Query (q to quit): ");
        scanf("%s", query);
        if (strcmp(query, "q") == 0) break;

        for (size_t i = 0; i < numbms; ++i) {
            if (!strstr(bms[i].url, query) || bms[i].count == 0) continue;
            printf("Opening matching url %s...\n", bms[i].url);
            sprintf(command, "%s -P %s %s", browser, profile, bms[i].url);
            system(command);
            while (1) {
                printf("Action: ");
                scanf("%c", &action);
                if (action == '\n') scanf("%c", &action);
                switch (action) {
                    case 'd':
                        bms[i].count = 0;
                        goto escapeaction;
                    case 's':
                        ++bms[i].count;
                        goto escapeaction;
                    case 'w':
                        goto escapeaction;
                    case 'q':
                        goto escapequery;
                    default:
                        break;
                }
            }
            escapeaction:;
        }
        escapequery:;
    }

    printf("Writing %lu bms to file %s...\n", numbms, bmsfile);
    write_bms(bms, numbms, bmsfile);

    free_bms(bms, numbms);
    return 0;
}
