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

#ifndef PROGRAM_H
#define PROGRAM_H

#include "cg.h"

extern struct mapping program_flags_mapping[];
extern struct mapping mpaa_mapping[];
extern struct mapping genre_mapping[];

struct program                  /* corresponds to an RTV Program */
{
/*  0 */ u32 struct_size;       /* structure size: 272 */
/*  4 */ u32 unknown1;          /* 1 */
/*  8 */ u32 isvalid;           /* If 0, nothing else in this is
                                   valid; should always be 1 in
                                   exported guieds */
/* 12 */ u32 tuning;            /* Channel to tune to (XXX verify) */
/* 16 */ struct fixed_program_record fixed_program_record;
}; /* 272 */

int parse_program(unsigned char **pp, struct program * prog);

void dump_program(struct program * p);

#endif
