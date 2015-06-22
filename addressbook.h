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

#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

extern struct mapping addressbook_entry_flag_bitmapping[];

struct addressbook_header {
/*  0 */ u32 version;           /* guess - treated as magic */
/*  4 */ u32 local_port;
/*  8 */ u32 remote_addresses;
}; /* 12 */

struct addressbook_entry 
{
/*  0 */ u8  name[21];
/* 21 */ u64 serial_number;     /* Actually only 48 bits, 6 bytes,
                                 * (with the top byte 0 in current
                                 * SNs */
/* 27 */ u8  unused1;
/* 28 */ u32 flags;             /* 1 = blocked */
}; /* 32 */

int parse_addressbook_header(unsigned char ** p, struct addressbook_header * h);
int parse_addressbook_entry(unsigned char ** p, struct addressbook_entry * e);

void dump_addressbook_header(struct addressbook_header * h);
void dump_isn(char * tag, u64 sn);
void dump_addressbook_entry(struct addressbook_entry * e);

#endif
