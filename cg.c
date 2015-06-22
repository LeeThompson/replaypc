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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtv.h"
#include "cg.h"
#include "dump.h"
#include "program.h"

int parse_parts(unsigned char **pp, struct parts * parts)
{
    unsigned char * p = *pp;
    
    parts->part = rtv_to_u16(&p);
    parts->of   = rtv_to_u16(&p);

    *pp = p;

    return 0;
}

int parse_movie(unsigned char **pp, struct movie * movie)
{
    unsigned char * p = *pp;
    
    movie->mpaa    = rtv_to_u16(&p);
    movie->stars   = rtv_to_u16(&p);
    movie->year    = rtv_to_u16(&p);
    movie->runtime = rtv_to_u16(&p);

    *pp = p;

    return 0;
}

int parse_fixed_program_record(unsigned char **pp,
                               struct fixed_program_record * prog,
                               int varsize)
{
    unsigned char * p = *pp;
    int data_len;
    u8 offset;
    
    memset(prog, 0, sizeof prog);
    
    prog->flags            = rtv_to_u32(&p);              
    prog->event_time       = rtv_to_u32(&p);
    prog->tmsid            = rtv_to_u32(&p);
    prog->minutes          = rtv_to_u16(&p);
    prog->genre1           = *p++;
    prog->genre2           = *p++;
    prog->genre3           = *p++;
    prog->genre4           = *p++;
    prog->record_len       = rtv_to_u16(&p);
    prog->title_len        = *p++;
    prog->episode_len      = *p++;
    prog->description_len  = *p++;
    prog->actor_len        = *p++;
    prog->guest_len        = *p++;
    prog->suzuki_len       = *p++;
    prog->producer_len     = *p++;
    prog->director_len     = *p++;
    if (varsize)
        data_len = prog->record_len - 28;
    else
        data_len = sizeof(prog->datablock);
    memcpy(prog->datablock, p, data_len);
    p                     += data_len;

    *pp = p;
    
    p = prog->datablock;
    if (prog->flags & 0x0040)
        parse_parts(&p, &prog->parts);
    if (prog->flags & 0x0020)
        parse_movie(&p, &prog->movie);
    offset = p - prog->datablock;
    prog->title_offset       = offset; offset += prog->title_len;
    prog->episode_offset     = offset; offset += prog->episode_len;
    prog->description_offset = offset; offset += prog->description_len;
    prog->actor_offset       = offset; offset += prog->actor_len;
    prog->guest_offset       = offset; offset += prog->guest_len;
    prog->suzuki_offset      = offset; offset += prog->suzuki_len;
    prog->producer_offset    = offset; offset += prog->producer_len;
    prog->director_offset    = offset; offset += prog->director_len;

    expect(prog->record_len == 0 ||
           ((offset + 28 + 3) & 0xfffc) == prog->record_len);

    return 0;
}

int parse_cg2_channel(unsigned char **pp, struct cg2_channel * chan)
{
    unsigned char * p = *pp;

    memset(chan, 0, sizeof *chan);
    
    chan->mod_time = rtv_to_u32(&p);
    chan->tmsid    = rtv_to_u32(&p);
    chan->data_len = rtv_to_u32(&p);

    *pp = p;

    return 0;
}

static int count_programs(unsigned char * p, struct cg2_channel * chan)
{
    unsigned char * start;
    unsigned char * tmp;
    u16 record_len;
    int count;

    count = 0;
    start = p;
    while (p < start + chan->data_len) {
        tmp = p + 18;
        record_len = rtv_to_u16(&tmp);
        p += record_len;
        count++;
    }

    return count;
}

int parse_cg2_programlist(unsigned char **pp, struct cg2_channel * chan)
{
    unsigned char * p = *pp;
    int i;

    chan->num_programs = count_programs(p, chan);
    chan->programs = calloc(chan->num_programs, sizeof *chan->programs);

    for (i = 0; i < chan->num_programs; i++)
        parse_fixed_program_record(&p, &chan->programs[i], 1);

    *pp = p;

    return 0;
}

void dump_parts(struct parts * p)
{
    dump_group_start    ("Parts");
    dump_u16            ("Part Number",         p->part);
    dump_u16            ("Total Parts",         p->of);
    dump_group_end      ();
}

void dump_movie(struct movie * p)
{
    dump_group_start    ("Movie");
    dump_bitmapping     ("MPAA Rating",       p->mpaa, mpaa_mapping);
    dump_u16            ("Stars Times Ten",   p->stars);
    dump_u16            ("Year",              p->year);
    dump_u16            ("Runtime",           p->runtime);
    dump_group_end      ();
}

void dump_fixed_program_record(struct fixed_program_record * p) 
{
    dump_group_start    ("Program");
    dump_bitmapping     ("Flags",        p->flags, program_flags_mapping);
    dump_time           ("Event Time",   p->event_time);
    dump_u32            ("TMS ID",       p->tmsid);
    dump_u16            ("Minutes",      p->minutes);
    if (p->genre1)
        dump_mapping    ("Genre 1",      p->genre1, genre_mapping);
    if (p->genre2)
        dump_mapping    ("Genre 2",      p->genre2, genre_mapping);
    if (p->genre3)
        dump_mapping    ("Genre 3",      p->genre3, genre_mapping);
    if (p->genre4)
        dump_mapping    ("Genre 4",      p->genre4, genre_mapping);
    dump_u16            ("Record Length",p->record_len);
    if (p->flags & 0x0040)
        dump_parts      (               &p->parts);
    if (p->flags & 0x0020)
        dump_movie      (               &p->movie);
    if (p->title_len > 1)
        dump_string     ("Title",        p->datablock + p->title_offset);
    if (p->episode_len > 1)
        dump_string     ("Episode",      p->datablock + p->episode_offset);
    if (p->description_len > 1)
        dump_string     ("Description",  p->datablock + p->description_offset);
    if (p->actor_len > 1)
        dump_string     ("Actor",        p->datablock + p->actor_offset);
    if (p->guest_len > 1)
        dump_string     ("Guest",        p->datablock + p->guest_offset);
    if (p->suzuki_len > 1)
        dump_string     ("Suzuki",       p->datablock + p->suzuki_offset);
    if (p->producer_len > 1)
        dump_string     ("Producer",     p->datablock + p->producer_offset);
    if (p->director_len > 1)
        dump_string     ("Director",     p->datablock + p->director_offset);
    dump_group_end      ();
}

void dump_cg2_channel(struct cg2_channel * p) 
{
    int i;
    
    dump_group_start    ("Channel Lineup");
    dump_time           ("Modification Time", p->mod_time);
    dump_u32            ("TMS ID",            p->tmsid);
    dump_u32            ("Data Length",       p->data_len);
    for (i = 0; i < p->num_programs; i++)
        dump_fixed_program_record(&p->programs[i]);
    dump_group_end      ();
}


