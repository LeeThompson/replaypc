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
#include "channelset.h"
#include "dump.h"

static int dump_channelset_file(FILE * fp)
{
    unsigned char header[256];
    unsigned char buffer[68];
    unsigned char * p;
    struct channelset_channel c;
    int i = 0;
    
    while (fread(buffer, 68, 1, fp)) {
        p = buffer;
        parse_channelset_channel(&p, &c);
        sprintf(header, "Channelset File entry #%d", i);
        dump_group_start(header);
        dump_channelset_channel(&c);
        dump_group_end();
        i++;
    }

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
            dump_channelset_file(fp);
            dump_group_end();
        }
    } else {
        dump_group_start("stdin");
        dump_channelset_file(stdin);
        dump_group_end();
    }

    return 0;
}
