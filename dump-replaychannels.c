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
#include "guide.h"
#include "dump.h"

struct mapping map[] = {
    {-1, NULL},
};
        
static int dump_replay_channel_file(FILE * fp)
{
    unsigned char buffer[624];
    unsigned char * p;
    struct replay_channel rc;
    
    while (fread(buffer, 624, 1, fp)) {
        p = buffer;
        parse_replay_channel(&p, &rc);
        dump_replay_channel(&rc, map);
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
            dump_replay_channel_file(fp);
            dump_group_end();
        }
    } else {
        dump_group_start("stdin");
        dump_replay_channel_file(stdin);
        dump_group_end();
    }

    return 0;
}
