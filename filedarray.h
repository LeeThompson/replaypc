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

#ifndef FILEDARRAY_H
#define FILEDARRAY_H

struct filedarray_header 
{
    u32 unknown1;
    u32 unknown2;
    u32 num_items;
    u32 item_size;
};

int parse_filedarray_header(unsigned char **pp,
                               struct filedarray_header * fah);

void dump_filedarray_header(struct filedarray_header * fah);

#endif
