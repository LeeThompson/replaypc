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
#include "dump.h"
#include "filedarray.h"

int parse_filedarray_header(unsigned char **pp,
                               struct filedarray_header * fah)
{
    unsigned char *p = *pp;

    fah->unknown1  = rtv_to_u32(&p);
    fah->unknown2  = rtv_to_u32(&p);
    fah->num_items = rtv_to_u32(&p);
    fah->item_size = rtv_to_u32(&p);

    *pp = p;

    return 0;
}

void dump_filedarray_header(struct filedarray_header * fah)
{
    dump_group_start    ("Filedarray header");
    dump_u32            ("Unknown1",            fah->unknown1);
    dump_u32            ("Unknown2",            fah->unknown2);
    dump_u32            ("Number of Items",     fah->num_items);
    dump_u32            ("Item Size",           fah->item_size);
    dump_group_end      ();
}

