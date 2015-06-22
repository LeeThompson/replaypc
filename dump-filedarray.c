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

#define MIN(x,y) ((x)<(y)?(x):(y))

#include <stdio.h>
#include "rtv.h"
#include "filedarray.h"
#include "dump.h"
#include "guide.h"

static int dump_filedarray_file(FILE * fp)
{
    struct filedarray_header fah;
    unsigned char            buffer[4096];
    unsigned char *          p;
    int                      i, count, items_left;
    
    fread(buffer, 4096, 1, fp);
    p = buffer;
    parse_filedarray_header(&p, &fah);
    dump_filedarray_header(&fah);

    items_left = fah.num_items;
    dump_group_start("Filedarray array");
    do {
        count = MIN(items_left, 4096/444);
        for (i = 0; i < count; i++) {
            switch(fah.item_size) {
                case 444: {
                    struct replay_show rs;
                    parse_replay_show(&p, &rs);
                    dump_replay_show(&rs);
                    break;
                }
                default:
                    fprintf(stderr, "Item size %ld not supported yet\n",
                            (unsigned long)fah.item_size);
                    break;
            }
        }
        items_left -= count;
    } while (fread(buffer, 4096, 1, fp) && (p = buffer, 1));

    dump_group_end();

    return 0;
}

int main(int argc, char ** argv)
{
    int i;
    
    dump_set_file(stdout);

    if (argc > 1) {
        for (i = 1; i < argc; i++) {
            FILE * fp;
            fp = fopen(argv[i], "r");
            if (!fp) {
                fprintf(stderr, "Couldn't open %s\n", argv[i]);
                return -1;
            }
            dump_group_start(argv[i]);
            dump_filedarray_file(fp);
            dump_group_end();
        }
    } else {
        dump_group_start("stdin");
        dump_filedarray_file(stdin);
        dump_group_end();
    }
    return 0;
}
