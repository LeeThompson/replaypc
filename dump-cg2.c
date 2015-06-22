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

#include <stdio.h>
#include "rtv.h"
#include "cg.h"
#include "dump.h"

static int read_and_dump_channel(FILE * in)
{
    struct cg2_channel chan;
    unsigned char headbuf[12];
    unsigned char * buf;
    unsigned char * p;
    
    if (fread(headbuf, 12, 1, in) <= 0)
        return -1;

    p = headbuf;
    parse_cg2_channel(&p, &chan);
    buf = malloc(chan.data_len);
    fread(buf, chan.data_len, 1, in);
    p = buf;
    parse_cg2_programlist(&p, &chan);
    expect(p == buf + chan.data_len);
    free(buf);
    
    dump_cg2_channel(&chan);

    return 0;
}

int main(int argc, char ** argv)
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
            while (read_and_dump_channel(fp) == 0)
                ;
            dump_group_end();
            fclose(fp);
        }
    } else {
        dump_group_start("stdin");
        while (read_and_dump_channel(stdin) == 0)
            ;
        dump_group_end();
    }
    
    return 0;
}
