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

#ifndef CHANNELSET_H
#define CHANNELSET_H

#include "headend.h"

struct channelset_channel {
/*  0 */ struct headend_channel headend_channel;
/* 56 */ u8 headend_name[8];
/* 64 */ u32 channelguide_index;
}; /* 68 */

int parse_channelset_channel(unsigned char ** pp, struct channelset_channel * c);
void dump_channelset_channel(struct channelset_channel * c);

#endif
