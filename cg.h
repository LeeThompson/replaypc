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

#ifndef CG_H
#define CG_H

struct movie 
{
/*  0 */ u16 mpaa;
/*  2 */ u16 stars;
/*  4 */ u16 year;
/*  6 */ u16 runtime;
};

struct parts
{
/*  0 */ u16 part;
/*  2 */ u16 of;
};

struct fixed_program_record     /* Corresponds to RTV FixedProgramRecord */
{
/*  0 */ u32 flags;             /* see Flags table */ 
/*  4 */ u32 event_time;        /* start of event, as time_t */
/*  8 */ u32 tmsid;             /* TMS ID of channel (XXX verify) */
/* 12 */ u16 minutes;
/* 14 */ u8  genre1;
/* 15 */ u8  genre2;
/* 16 */ u8  genre3;
/* 17 */ u8  genre4;
/* 18 */ u16 record_len;
/* 20 */ u8  title_len;
/* 21 */ u8  episode_len;
/* 22 */ u8  description_len;
/* 23 */ u8  actor_len;
/* 24 */ u8  guest_len;
/* 25 */ u8  suzuki_len;
/* 26 */ u8  producer_len;
/* 27 */ u8  director_len;
/* 28 */ u8  datablock[228];

    /* Convenience members: these don't directly correspond to
       items in the file */
    struct parts parts;         /* valid only if flags & 0x040 */
    struct movie movie;         /* valid only if flags & 0x020 */
    u8   title_offset;
    u8   episode_offset;
    u8   description_offset;
    u8   actor_offset;
    u8   guest_offset;
    u8   suzuki_offset;
    u8   producer_offset;
    u8   director_offset;
}; /* 256 */

struct cg2_channel
{
/*  0 */ u32 mod_time;
/*  4 */ u32 tmsid;
/*  8 */ u32 data_len;

    /* Not part of the base struct: */
    int num_programs;
    struct fixed_program_record * programs;
}; /* 12 */

int parse_parts(unsigned char **p, struct parts * parts);
int parse_movie(unsigned char **p, struct movie * movie);
int parse_fixed_program_record(unsigned char **pp, struct fixed_program_record * prog, int varsize);
int parse_cg2_channel(unsigned char **pp, struct cg2_channel * chan);
int parse_cg2_programlist(unsigned char **pp, struct cg2_channel * chan);

void dump_parts(struct parts * p);
void dump_movie(struct movie * p);
void dump_fixed_program_record(struct fixed_program_record * prog);
void dump_cg2_channel(struct cg2_channel * chan);

#endif
