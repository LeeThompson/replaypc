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
#include "channelset.h"
#include "dump.h"

int parse_channelset_channel(unsigned char ** pp, struct channelset_channel * c)
{
    unsigned char * p = *pp;

    parse_headend_channel(&p, &c->headend_channel);
    memcpy(c->headend_name, p, 8);  p += 8;
    c->channelguide_index = rtv_to_u32(&p);

    *pp = p;

    return 0;
}

void dump_channelset_channel(struct channelset_channel * c)
{
    dump_group_start    ("Channelset Channel");
    dump_headend_channel(&c->headend_channel);
    dump_string         ("Headend Name",        c->headend_name);
    dump_u32            ("ChannelGuide Index",  c->channelguide_index);
    dump_group_end      ();
}

