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
#include "addressbook.h"
#include "dump.h"

static int dump_addressbook_file(FILE * fp)
{
    unsigned char buffer[32];
    unsigned char * p;
    unsigned int i;
    struct addressbook_header h;
    struct addressbook_entry e;

    fread(buffer, 12, 1, fp);
    p = buffer;
    parse_addressbook_header(&p, &h);
    dump_addressbook_header(&h);

    dump_group_start("Addressbook Entries");

    dump_group_start("Local Entry");
    fread(buffer, 32, 1, fp);
    p = buffer;
    parse_addressbook_entry(&p, &e);
    dump_addressbook_entry(&e);
    dump_group_end();

    dump_group_start("Remote Entries");
    for (i = 0; i < h.remote_addresses; i++) {
        fread(buffer, 32, 1, fp);
        p = buffer;
        parse_addressbook_entry(&p, &e);
        dump_addressbook_entry(&e);
    }
    dump_group_end();

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
            dump_addressbook_file(fp);
            dump_group_end();
        }
    } else {
        dump_group_start("stdin");
        dump_addressbook_file(stdin);
        dump_group_end();
    }

    return 0;
}
