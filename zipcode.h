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

#ifndef ZIPCODE_H
#define ZIPCODE_H

extern struct mapping headend_type_mapping[];

struct zipcode2_headend {
/*  0 */ u8  name[8];
/*  8 */ u8  description[56];
/* 64 */ u8  type;
/* 65 */ u8  unk1;
/* 66 */ u16 unk2;
/* 68 */ u8  extra_data[56];
}; /* 124 */

int parse_zipcode2_headend(unsigned char ** pp, struct zipcode2_headend * zh);
void dump_zipcode2_headend(struct zipcode2_headend * zh);

#endif
