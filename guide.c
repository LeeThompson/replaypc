/*
 * Copyright (C) 2002 John Todd Larason <jtl@molehill.org>,
 * Lee Thompson <thompsonl@logh.net> and Dan Frumin <rtv@frumin.com>
 *
 * This is based, mostly indirectly, on Lee Thompson's GuideParser
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
#include "guide.h"
#include "cg.h"
#include "dump.h"
#include "headend.h"
#include "program.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define DUMP_EVERYTHING 0
/* File format:
   guide_header
   group_data
   replay_channel ...
   replay_show ...
*/

static unsigned char lots_of_zeros[8192] = {0};

int parse_guide_header(unsigned char ** pp, struct guide_header * h) 
{
    unsigned char * p;
    
    p = *pp;
    h->version                   = rtv_to_u32(&p);
    h->struct_size               = rtv_to_u32(&p);
    h->num_channels_1            = rtv_to_u32(&p);
    h->num_channels_2            = rtv_to_u32(&p);
    h->group_data_offset         = rtv_to_u32(&p);
    h->channel_offset            = rtv_to_u32(&p);
    h->show_offset               = rtv_to_u32(&p);
    h->flags                     = rtv_to_u32(&p);

    *pp = p;

    /* The flags member really is flags, but in this version fo the
       format/software, only the lowest bit is used, and the others
       aren't initialized */

    h->flags &= 1;
    
    expect(h->version == 0x00030001);
    expect(h->struct_size == 32);
    expect(h->num_channels_1 == h->num_channels_2);
    expect(h->group_data_offset == 32);
    expect(h->channel_offset == h->group_data_offset + 776);
    expect(h->show_offset ==
           h->group_data_offset + 776 + 624 * h->num_channels_1);

    return 0;
}

int parse_group_data(unsigned char ** pp, struct group_data * gd) 
{
    unsigned char * p = *pp;
    unsigned int i;
    
    gd->struct_size               = rtv_to_u32(&p);
    gd->num_categories            = rtv_to_u32(&p);
    for (i = 0; i < 32; i++)
        gd->index[i]              = rtv_to_u32(&p);
    for (i = 0; i < 32; i++)
        gd->name_offset[i]        = rtv_to_u32(&p);
    memcpy(gd->name_block, p, sizeof gd->name_block);
    p += sizeof gd->name_block;

    *pp = p;

    expect(gd->struct_size == 776);
    expect(gd->num_categories <= 32);

    for (i = 0; i < gd->num_categories; i++) {
        gd->category_bitmapping[i].value = 1 << gd->index[i];
        gd->category_bitmapping[i].name  =
            gd->name_block + gd->name_offset[i];
    }
    gd->category_bitmapping[gd->num_categories].value   = 1 << 16;
    gd->category_bitmapping[gd->num_categories].name    = "Received";
    gd->category_bitmapping[gd->num_categories+1].value = -1;
    gd->category_bitmapping[gd->num_categories+1].name  = NULL;

    return 0;
}

int parse_channel(unsigned char **pp, struct channel * c)
{
    unsigned char * p = *pp;
    
    c->struct_size    = rtv_to_u32(&p);
    c->unknown1       = rtv_to_u32(&p);
    c->isvalid        = rtv_to_u32(&p);
    parse_headend_channel(&p, &c->headend_channel);
    memcpy(c->headend, p, sizeof c->headend);         p += sizeof c->headend;
    c->guide_index    = rtv_to_u32(&p);

    if (c->struct_size) {
        expect(c->struct_size == 80);
        expect(c->unknown1 == 1);
        expect(c->isvalid == 1);
    } else {
        if (!c->isvalid) {
            expect(memcmp(c, lots_of_zeros, sizeof *c) == 0);
        }
    }
    
    *pp = p;
    
    return 0;
}

int parse_replay_show(unsigned char **pp, struct replay_show * rs)
{
    unsigned char * p = *pp;

    rs->channel_id       = rtv_to_u32(&p);
    rs->show_id          = rtv_to_u32(&p);
    rs->input_source     = rtv_to_u32(&p);
    rs->quality          = rtv_to_u32(&p);
    rs->guaranteed       = rtv_to_u32(&p);
    rs->flags            = rtv_to_u32(&p);
    parse_channel(&p, &rs->channel);
    parse_program(&p, &rs->program);
    rs->ivs_status       = rtv_to_u32(&p);
    rs->remote_guide_id  = rtv_to_u32(&p);
    rs->download_id      = rtv_to_u32(&p);
    rs->times_sent       = rtv_to_u32(&p);
    rs->seconds          = rtv_to_u32(&p);
    rs->gop_count        = rtv_to_u32(&p);
    rs->gop_highest_seen = rtv_to_u32(&p);
    rs->gop_last_seen    = rtv_to_u32(&p);
    rs->checkpointed     = rtv_to_u32(&p);
    rs->intact           = rtv_to_u32(&p);
    rs->upgrade_flag     = rtv_to_u32(&p);
    rs->instance         = rtv_to_u32(&p);
    rs->unused1          = rtv_to_u16(&p);
    rs->before_padding   = *p++;
    rs->after_padding    = *p++;
    rs->index_size       = rtv_to_u64(&p);
    rs->mpeg_size        = rtv_to_u64(&p);

    expect(rs->program.fixed_program_record.tmsid ==
           rs->channel.headend_channel.tmsid);

    *pp = p;

    return 0;
}

int parse_replay_theme(unsigned char **pp, struct replay_theme * rt)
{
    unsigned char * p = *pp;
    
    rt->flags            = rtv_to_u32(&p);
    rt->suzuki_id        = rtv_to_u32(&p);
    rt->minutes          = rtv_to_u32(&p);
    memcpy(rt->keywords, p, sizeof rt->keywords);
    p += sizeof rt->keywords;

    *pp = p;

    return 0;
}

int parse_replay_channel(unsigned char **pp, struct replay_channel * rc)
{
    unsigned char * p = *pp;
    
    rc->channel_type     = rtv_to_u32(&p);
    rc->quality          = rtv_to_u32(&p);
    rc->space_allocated  = rtv_to_u64(&p);
    rc->keep             = rtv_to_u32(&p);
    rc->stored           = rtv_to_u32(&p);
    rc->days_of_week     = *p++;
    rc->after_padding    = *p++;
    rc->before_padding   = *p++;
    rc->flags            = *p++;
    rc->category         = rtv_to_u32(&p);
    rc->channel_id       = rtv_to_u32(&p);
    rc->unknown1         = rtv_to_u32(&p);
    rc->unknown2         = rtv_to_u32(&p);
    rc->unknown3         = rtv_to_u32(&p);
    rc->unknown4         = rtv_to_u32(&p);
    rc->unknown5         = rtv_to_u32(&p);
    rc->unknown6         = rtv_to_u32(&p);
    rc->unknown7         = rtv_to_u32(&p);
    rc->seconds_reserved = rtv_to_u32(&p);
    memcpy(rc->label, p, sizeof rc->label); p += sizeof rc->label;
    parse_replay_show(&p, &rc->replay_show);
    parse_replay_theme(&p, &rc->replay_theme);

    *pp = p;

    return 0;
}

int parse_replay_guide_snapshot(unsigned char **pp,
                                struct replay_guide_snapshot * ss)
{
    unsigned int i;
    unsigned char * p = *pp;
    unsigned char * start = p;
    
    if (parse_guide_header(&p, &ss->guide_header) < 0) {
        fprintf(stderr, "Error parsing guide header\n");
        return -1;
    }
    expect(p == start + ss->guide_header.group_data_offset);

    if (parse_group_data(&p, &ss->group_data) < 0) {
        fprintf(stderr, "Error parsing group data\n");
        return -1;
    }
    expect(p == start + ss->guide_header.channel_offset);

    ss->num_shows = 0;
    ss->replay_channels = calloc(ss->guide_header.num_channels_1,
                                 sizeof(struct replay_channel));
    for (i = 0; i < ss->guide_header.num_channels_1; i++) {
        if (parse_replay_channel(&p, &ss->replay_channels[i]) < 0) {
            fprintf(stderr, "Error parsing replay channel %d\n", i);
            return -1;
        }
        ss->num_shows += ss->replay_channels[i].stored;
    }
    expect(p == start + ss->guide_header.show_offset);

    ss->replay_shows = calloc(ss->num_shows, sizeof(struct replay_show));
    for (i = 0; i < ss->num_shows; i++) {
        if (parse_replay_show(&p, &ss->replay_shows[i]) < 0) {
            fprintf(stderr, "Error parsing replay show %d\n", i);
            return -1;
        }
    }
    
    return 0;
}

int parse_replay_guide_snapshot_staged(unsigned char **pp,
                                       struct replay_guide_snapshot * ss)
{
    unsigned int i;
    int num_shows = 0;
    
    switch(ss->stage) {
      case 0:                   /* Just starting -- nothing to do but
                                 * read the header  */
        ss->stage = 1;
        return 32;
      case 1:                   /* Parse the header, and read the
                                 * guide data */
        if (parse_guide_header(pp, &ss->guide_header) < 0) {
            fprintf(stderr, "Error parsing guide header\n");
            return -1;
        }
        ss->guide_header.num_channels_2 = 0;
        ss->num_shows = 0;
        ss->stage = 2;
        return 776;
      case 2:                   /* Parse the guide data, read the
                                 * channels */
        if (parse_group_data(pp, &ss->group_data) < 0) {
            fprintf(stderr, "Error parsing group data\n");
            return -1;
        }
        ss->stage = 3;
        return ss->guide_header.num_channels_1 * 624;
      case 3:                   /* Parse the channels, then find the
                                 * first one with any stored programs,
                                 * and read those */
        ss->replay_channels = calloc(ss->guide_header.num_channels_1,
                                     sizeof(struct replay_channel));
        for (i = 0; i < ss->guide_header.num_channels_1; i++) {
            if (parse_replay_channel(pp, &ss->replay_channels[i]) < 0) {
                fprintf(stderr, "Error parsing replay channel %d\n", i);
                return -1;
            }
            num_shows += ss->replay_channels[i].stored;
        }
        ss->replay_shows = calloc(num_shows, sizeof(struct replay_show));
        for (i = 0; i < ss->guide_header.num_channels_1; i++) {
            if (ss->replay_channels[i].stored) {
                ss->stage = 4;
                ss->guide_header.num_channels_2 = i;
                return ss->replay_channels[i].stored * 444;
            } else {
                fprintf(stderr, "Skipping channel %d: %ld\n",
                        i, ss->replay_channels[i].stored);
            }
        }
        ss->stage = 5;
        ss->guide_header.num_channels_2 = i;
        return 0;
      case 4:                   /* Read one channel's worth of
                                 * programs, then find the next
                                 * channel with programs to read; if
                                 * there aren't any, we're done */
        for (i = 0;
             i < ss->replay_channels[ss->guide_header.num_channels_2].stored;
             i++) {
            if (parse_replay_show(pp, &ss->replay_shows[ss->num_shows]) < 0) {
                fprintf(stderr, "Error parsing replay show %d\n", i);
                return -1;
            }
            ss->num_shows++;
        }
        for (i = ss->guide_header.num_channels_2+1;
             i < ss->guide_header.num_channels_1;
             i++) {
            if (ss->replay_channels[i].stored) {
                ss->guide_header.num_channels_2 = i;
                return ss->replay_channels[i].stored * 444;
            }
        }
        ss->stage = 5;
        ss->guide_header.num_channels_2 = i;
        return 0;
      default:
        expect(0);
        return 0;
    }
}

void free_replay_guide_snapshot(struct replay_guide_snapshot * ss)
{
    free(ss->replay_channels); ss->replay_channels = NULL;
    free(ss->replay_shows);    ss->replay_shows    = NULL;
}

struct mapping replay_channel_type_mapping[] = {
    {1, "Recurring"},
    {2, "Theme"},               /* extra data is theme info */
    {3, "Single"},
    {4, "Zone"},                /* extra data is theme info */
    {-1, NULL}
};

struct mapping quality_mapping[] = {
    {0, "High"},
    {1, "Medium"},
    {2, "Standard"},
    {-1, NULL}
};

struct mapping input_source_mapping[] = {
    {0, "Raw RF"},
    {1, "Line 1 Direct"},
    {2, "Line 2 Direct"},
    {3, "Tuner"},
    {-1, NULL}
};

struct mapping show_flags_bitmapping[] = {
    { 1 << 0, "Manual record"},
    { 1 << 1, "AutoSkip on"},
    { 1 << 2, "AutoSkip explicitely set"},
    { 1 << 4, "Manual record of Pay-Per-View"},
    { -1, NULL}
};

struct mapping ivs_status_mapping[] = {
    {0, "Local"},                               /* 0b0000 */
    {1, "LAN"},                                 /* 0b0001 */
    {2, "Internet Downloadable"},               /* 0b0010 */
    {3, "Internet Download Failed"},            /* 0b0011 */
    {4, "Internet Index Download Restart"},     /* 0b0100 */
    {5, "Internet Index Downloading"},          /* 0b0101 */
    {6, "Internet Index Download Complete"},    /* 0b0110 */
    {7, "Internet MPEG Download Restart"},      /* 0b0111 */
    {8, "Internet MPEG Downloading"},           /* 0b1000 */
    {9, "Internet MPEG Download Complete"},     /* 0b1001 */
    {10, "Internet File Not Found"},            /* 0b1010 */
    { -1, NULL}
};

struct mapping channel_flags_bitmapping[] = {
    {1 << 0, "Raw Mode"},
    {1 << 1, "IVS"},
    {1 << 2, "Exported"},
    {-1, NULL}
};

struct mapping day_of_week_bitmapping[] = {
    {1 << 0, "Sun"},
    {1 << 1, "Mon"},
    {1 << 2, "Tue"},
    {1 << 3, "Wed"},
    {1 << 4, "Thu"},
    {1 << 5, "Fri"},
    {1 << 6, "Sat"},
    {-1, NULL}
};

struct mapping theme_flags_bitmapping[] = {
    {1 << 0, "Actor search"},
    {1 << 1, "Director search"},
    {1 << 2, "Title search"},
    {1 << 3, "Title match"},
    {1 << 4, "Description search"},
    {1 << 5, "Random"},
    {-1, NULL},
};

struct mapping guide_flags_bitmapping[] = {
    {1 << 0, "AutoSkip defaults to on"},
    {-1, NULL},
};


void dump_guide_header(struct guide_header * h) 
{
    dump_group_start    ("Replay Guide Header");
    dump_u32            ("Version",                 h->version);
    dump_u32            ("Struct Size",             h->struct_size);
    dump_u32            ("Num Channels #1",         h->num_channels_1);
    dump_u32            ("Num Channels #2",         h->num_channels_2);
    dump_u32            ("Group Data Offset",       h->group_data_offset);
    dump_u32            ("Channel Offset",          h->channel_offset);
    dump_u32            ("Show Offset",             h->show_offset);
    dump_bitmapping     ("Flags",                   h->flags, guide_flags_bitmapping);
    dump_group_end      ();
}

void dump_group_data(struct group_data * gd)
{
    unsigned int i;
    char buffer[32];

    dump_group_start    ("Group Data");
    dump_u32            ("Struct Size",             gd->struct_size);
    dump_u32            ("Num Categories",          gd->num_categories);
    for (i = 0; i < gd->num_categories; i++) {
        sprintf         (buffer, "Category Index %d", i);
        dump_u32        (buffer,                    gd->index[i]);
    }
    for (i = 0; i < gd->num_categories; i++) {
        sprintf(buffer, "Category Name %d", i);
        dump_string     (buffer,                    gd->name_block + gd->name_offset[i]);
    }
    dump_group_end      ();
}

void dump_channel(struct channel * c)
{
    if (!c->isvalid)
        return;

    dump_group_start    ("Channel");
    dump_u32            ("Struct Size",          c->struct_size);
    dump_u32            ("Unknown1",             c->unknown1);
    dump_u32            ("Is Valid",             c->isvalid);
    dump_headend_channel(                       &c->headend_channel);
    dump_string         ("Headend",              c->headend);
    dump_u32            ("Guide Index",          c->guide_index);
    dump_group_end      ();
}

void dump_replay_show(struct replay_show * rs)
{
    dump_group_start    ("Replay Show");
    dump_time           ("Channel ID",          rs->channel_id);
    dump_time           ("Show ID",             rs->show_id);
    dump_mapping        ("Input Source",        rs->input_source,       input_source_mapping);
    dump_mapping        ("Quality",             rs->quality,            quality_mapping);
    dump_u32            ("Guaranteed",          rs->guaranteed);
    dump_bitmapping     ("Flags",               rs->flags, show_flags_bitmapping);
    dump_channel        (                      &rs->channel);
    dump_program        (                      &rs->program);
    dump_mapping        ("IVS Status",          rs->ivs_status,         ivs_status_mapping);
    dump_u32            ("Remote Guide ID",     rs->remote_guide_id);
    dump_u32            ("Download ID",         rs->download_id);
    dump_u32            ("Times Sent",          rs->times_sent);
    dump_u32            ("Seconds",             rs->seconds);
    dump_u32            ("GOP Count",           rs->gop_count);
    dump_u32            ("GOP Highest Seen",    rs->gop_highest_seen);
    dump_u32            ("GOP Last Seen",       rs->gop_last_seen);
    dump_u32            ("Checkpointed",        rs->checkpointed);
    dump_u32            ("Intact",              rs->intact);
    dump_u32            ("Upgrade Flag",        rs->upgrade_flag);
    dump_u32            ("Instance",            rs->instance);
    dump_u16            ("Unused1",             rs->unused1);
    dump_u8             ("Padding Before",      rs->before_padding);
    dump_u8             ("Padding After",       rs->after_padding);
    dump_u64            ("Index Size",          rs->index_size);
    dump_u64            ("MPEG Size",           rs->mpeg_size);
    dump_group_end      ();
}

void dump_replay_theme(struct replay_theme * rt)
{
    dump_group_start    ("Replay Theme");
    dump_bitmapping     ("Flags",                rt->flags, theme_flags_bitmapping);
    dump_u32            ("Suzuki ID",            rt->suzuki_id);
    dump_u32            ("Minutes",              rt->minutes);
    dump_string         ("Keywrods",             rt->keywords);
    dump_group_end      ();
}

void dump_replay_channel(struct replay_channel * rc, struct mapping * category_bitmapping)
{
    dump_group_start    ("Guide Channel");
    dump_mapping        ("Guide Channel Type",   rc->channel_type, replay_channel_type_mapping);
    dump_mapping        ("quality",              rc->quality, quality_mapping);
    dump_u64            ("space_allocated",      rc->space_allocated);
    dump_u32            ("keep",                 rc->keep);
    dump_u32            ("stored",               rc->stored);
    dump_bitmapping     ("days_of_week",         rc->days_of_week, day_of_week_bitmapping);
    dump_u8             ("after_padding",        rc->after_padding);
    dump_u8             ("before_padding",       rc->before_padding);
    dump_bitmapping     ("flags",                rc->flags, channel_flags_bitmapping);
    dump_bitmapping     ("category",             rc->category, category_bitmapping);
    dump_time           ("Channel ID",           rc->channel_id);
    dump_time           ("unknown1"     ,        rc->unknown1);
    dump_u32            ("unknown2",             rc->unknown2);
    dump_u32            ("unknown3",             rc->unknown3);
    dump_u32            ("unknown4",             rc->unknown4);
    dump_u32            ("unknown5",             rc->unknown5);
    dump_u32            ("unknown6",             rc->unknown6);
    dump_u32            ("unknown7",             rc->unknown7);
    dump_u32            ("seconds_reserved",     rc->seconds_reserved);
    dump_string         ("label",                rc->label);
    dump_replay_show    (                       &rc->replay_show);
    dump_replay_theme   (                       &rc->replay_theme);
    dump_group_end      ();
}

void dump_replay_guide_snapshot(struct replay_guide_snapshot * ss)
{
    unsigned int i;
    
    dump_group_start            ("Replay Guide Snapshot");
    dump_guide_header           (&ss->guide_header);
    dump_group_data             (&ss->group_data);
    dump_group_start            ("Replay Channels");
    for (i = 0; i < ss->guide_header.num_channels_1; i++) {
        dump_replay_channel     (&ss->replay_channels[i], ss->group_data.category_bitmapping);
    }
    dump_group_end              ();
    dump_group_start            ("Replay Shows");
    for (i = 0; i < ss->num_shows; i++) {
        dump_replay_show        (&ss->replay_shows[i]);
    }
    dump_group_end              ();
    dump_group_end              ();
}
