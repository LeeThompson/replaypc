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
//#include "programset.h"
#include "dump.h"

static int dump_programset_file(FILE * fp)
{
    unsigned char header[256];
    unsigned char buffer[256];
    unsigned char * p;
    struct fixed_program_record r;
    int i = 0;

    {
        u32 tms_id;
        u32 unk1;
        fread(buffer, 8, 1, fp);
        p      = buffer;
        tms_id = rtv_to_u32(&p);
        unk1   = rtv_to_u32(&p);
        
        dump_group_start("Programset File Header");
        dump_u32        ("TMS ID", tms_id);
        dump_u32        ("Unknown 1", unk1);
    }
    
    while (fread(buffer, 256, 1, fp)) {
        p = buffer;
        parse_fixed_program_record(&p, &r);
        sprintf(header, "FPR #%d", i);
        dump_group_start(header);
        dump_fixed_program_record(&r);
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
            dump_programset_file(fp);
            dump_group_end();
        }
    } else {
        dump_group_start("stdin");
        dump_programset_file(stdin);
        dump_group_end();
    }

    return 0;
}
