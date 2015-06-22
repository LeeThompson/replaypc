/*
 * Copyright (C) 2002 John Todd Larason <jtl@molehill.org>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 */

#include "guide.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dump_guide(FILE * fp)
{
    struct replay_guide_snapshot ss;
    unsigned char * buffer;
    unsigned char * p;
    size_t l;
    fseek(fp, 0, SEEK_END);
    l = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buffer = malloc(l);
    fread(buffer, l, 1, fp);
    p = buffer;
    parse_replay_guide_snapshot(&p, &ss);
    dump_replay_guide_snapshot(&ss);
    free_replay_guide_snapshot(&ss);
}

static void dump_guide_parts(FILE * fp)
{
    struct replay_guide_snapshot ss;
    unsigned char * buffer;
    unsigned char * p;
    size_t l;
    size_t buffer_size;

    buffer_size = 0;
    buffer = NULL;
    memset(&ss, 0, sizeof ss);

    p = NULL;
    while ((l = parse_replay_guide_snapshot_staged(&p, &ss)) > 0) {
        if (l > buffer_size) {
            free(buffer);
            buffer = malloc(l);
        }
        fread(buffer, l, 1, fp);
        p = buffer;
    }
    free(buffer);
    dump_replay_guide_snapshot(&ss);
    free_replay_guide_snapshot(&ss);
}

int main (int argc, char ** argv)
{
    dump_set_file(stdout);
    if (argc > 1) {
        int i;
        FILE * fp;
        for (i = 1; i < argc; i++) {
            fp = fopen(argv[i], "r");
            if (!fp) {
                fprintf(stderr, "Couldn't open %s\n", argv[i]);
                exit(-1);
            }
            dump_group_start(argv[i]);
            dump_guide(fp);
            dump_group_end();
            fclose(fp);
        }
    } else {
#if 0
        printf("Sorry, you can't dump guides from stdin yet.\n");
#else
        dump_group_start("stdin");
        dump_guide_parts(stdin);
        dump_group_end();
#endif
    }
    
    return 0;
}
