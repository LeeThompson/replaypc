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

#ifndef NDX_H
#define NDX_H

#include "rtv.h"
#include <stdio.h>

struct ndx_record 
{
/*  0 */ u8  flag_1;
/*  1 */ u8  commercial_flag;
/*  2 */ u16 video_offset; /* relative to stream_position */
/*  4 */ u8  unk_fe;
/*  5 */ u8  macrovision;
/*  6 */ u16 macrovision_count;
/*  8 */ u32 audio_offset; /* relative to stream_position + video_offset */
/* 12 */ u32 unused1;      /* always 0 */
/* 16 */ u64 timestamp;    /* seconds * 10e9, from an unknown base */
/* 24 */ u64 stream_position;
}; /* 32 */

extern int read_ndx_record(FILE * fp, struct ndx_record * pr);
extern int write_ndx_record(FILE * fp, struct ndx_record * pr);

#endif
