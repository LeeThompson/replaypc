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

#include <stdlib.h>
#include <string.h>
#include "rtv.h"
#include "zipcode.h"
#include "dump.h"

struct mapping headend_type_mapping[] = {
    { 'C', "Cable"},
    { 'S', "Satellite"},
    { 'A', "Antenna"},
    { -1, NULL}
};

int parse_zipcode2_headend(unsigned char ** pp, struct zipcode2_headend * zh)
{
    unsigned char * p = *pp;

    memcpy(zh->name, p, 8);             p += 8;
    memcpy(zh->description, p, 56);     p += 56;
    zh->type = *p++;
    zh->unk1 = *p++;
    zh->unk2 = rtv_to_u16(&p);
    memcpy(zh->extra_data, p, 56);      p += 56;
    
    *pp = p;
    
    return 0;
}

void dump_zipcode2_headend(struct zipcode2_headend * zh)
{
    dump_group_start    ("Zipcode2 Headend");
    dump_string         ("Name",                zh->name);
    dump_string         ("Description",         zh->description);
    dump_mapping        ("Type",                zh->type, headend_type_mapping);
    dump_u8             ("Unknown 1",           zh->unk1);
    dump_u16            ("Unknown 16",          zh->unk2);
    dump_string         ("Extra Data",          zh->extra_data);
    dump_group_end      ();
#if 0
    printf("%-8s %-40s %-10s %s\n",
           headend, description, type_name(type), extra_data);
#endif
}

