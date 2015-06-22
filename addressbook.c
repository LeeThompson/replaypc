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

#include "rtv.h"
#include "addressbook.h"
#include "dump.h"
#include <string.h>

int parse_addressbook_header(unsigned char ** pp, struct addressbook_header * h)
{
    unsigned char * p = *pp;

    h->version          = rtv_to_u32(&p);
    h->local_port       = rtv_to_u32(&p);
    h->remote_addresses = rtv_to_u32(&p);

    *pp = p;

    return 0;
}
    
int parse_addressbook_entry(unsigned char ** pp, struct addressbook_entry * e)
{
    unsigned char * p = *pp;

    memcpy(e->name, p, 22);                    p += 21;
    e->serial_number  = (u64)rtv_to_u16(&p) << 32;
    e->serial_number |= rtv_to_u32(&p);
    e->unused1        = *p++;
    e->flags          = rtv_to_u32(&p);

    *pp = p;

    return 0;
}

struct mapping addressbook_entry_flag_bitmapping[] = {
    { 1 << 0, "Blocked" },
    { -1, NULL },
};
        


void dump_addressbook_header(struct addressbook_header * h)
{
    dump_group_start            ("Addressbook Header");
    dump_u32                    ("Version",             h->version);
    dump_u32                    ("Local Port",          h->local_port);
    dump_u32                    ("Remote Address Count",h->remote_addresses);
    dump_group_end              ();
}

void dump_isn(char * tag, u64 sn)
{
    int part1, part2, part3;
    char buf[32];

    part1 = sn/10000000000 % 100000;
    part2 = sn/100000      % 100000;
    part3 = sn             % 100000;

    sprintf(buf, "%05d-%05d-%05d", part1, part2, part3);

    dump_string(tag, buf);
}

void dump_addressbook_entry(struct addressbook_entry * e)
{
    dump_group_start            ("Addressbook Entry");
    dump_string                 ("Name",                e->name);
    dump_isn                    ("ISN",                 e->serial_number);
    dump_u8                     ("Unused1",             e->unused1);
    dump_bitmapping             ("Flags",               e->flags, addressbook_entry_flag_bitmapping);
    dump_group_end              ();
}

