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

#ifndef GUIDE_H
#define GUIDE_H

#include <stdlib.h>
#include "dump.h"
#include "headend.h"
#include "program.h"

extern struct mapping replay_channel_type_mapping[];
extern struct mapping quality_mapping[];
extern struct mapping input_source_mapping[];
extern struct mapping show_flags_bitmapping[];
extern struct mapping record_show_flags_bitmapping[];
extern struct mapping channel_flags_bitmapping[];
extern struct mapping day_of_week_bitmapping[];
extern struct mapping theme_flags_bitmapping[];
extern struct mapping guide_flags_bitmapping[];

struct guide_header {                /* unknown name on RTV */
/*  0 */ u32 version;                /* 0x30001 -- magic, guess @ meaning */
/*  4 */ u32 struct_size;            /* 0x0020  -- magic, guess @ meaning */   
/*  8 */ u32 num_channels_1;         /* number of replay channels, copy #1 */
/* 12 */ u32 num_channels_2;         /* number of replay channels, copy #2 */
/* 16 */ u32 group_data_offset;      /* offset of group_data struct */
/* 20 */ u32 channel_offset;         /* offset of first replay channel */
/* 24 */ u32 show_offset;            /* offset of first replay show */
/* 28 */ u32 flags;                  /* see guide_flags_bitmapping and comment
                                        in parse_guide_header */
}; /* 32 */

struct group_data {                  /* corresponds to RTV guide_data */
/*  0 */ u32 struct_size;            /* 776 */
/*  4 */ u32 num_categories;         /* number of categories, max 32 */
/*  8 */ u32 index[32];
/*136 */ u32 name_offset[32];
/*264 */ u8  name_block[512];

    /* convenience -- doesn't correspond */
    struct mapping category_bitmapping[32];

};/*776 */

struct channel {                     /* corresponds to an RTV Channel */
/*  0 */ u32 struct_size;            /* structure size: 80 */
/*  4 */ u32 unknown1;               /* 1 */
/*  8 */ u32 isvalid;                /* If 0, nothing else in this is
                                      * valid; should always be 1 in
                                      * exported guides */
/* 12 */ struct headend_channel headend_channel;
/* 68 */ u8  headend[8];
/* 76 */ u32 guide_index;            /* the channel # as shown in the guide */
}; /* 80 */

struct replay_show {                 /* corresponds to an RTV ReplayShow */
/*  0 */ u32 channel_id;             /* channel ID, channelcreate time */
/*  4 */ u32 show_id;                /* show ID, start of recording */
/*  8 */ u32 input_source;           /* see input_source_mapping */
/* 12 */ u32 quality;                /* see quality_mapping */
/* 16 */ u32 guaranteed;             /* 0 or all Fs */
/* 20 */ u32 flags;                  /* see show_flags_bitmapping */
/* 24 */ struct channel channel;
/*104 */ struct program program;
/*376 */ u32 ivs_status;             /* see ivs_status_mapping */
/*380 */ u32 remote_guide_id;        /* always 0 on exported? -- for LAN or IVS received, show_id on originating system */
/*384 */ u32 download_id;
/*388 */ u32 times_sent;
/*392 */ u32 seconds;                /* actual recorded duration */
/*396 */ u32 gop_count;        
/*400 */ u32 gop_highest_seen;
/*404 */ u32 gop_last_seen;
/*408 */ u32 checkpointed;           /* if 0, the other fields may be
                                      * out of date */
/*412 */ u32 intact;                 /* 0 = don't put in snapshot file */
/*416 */ u32 upgrade_flag;           /* never exposed as non-0 */
/*420 */ u32 instance;
/*424 */ u16 unused1;                /* see ReplayShow::SetShowGuardMinutes */
/*426 */ u8  before_padding;
/*427 */ u8  after_padding;
/*428 */ u64 index_size;
/*436 */ u64 mpeg_size;
}; /* 444 */

/* if suzuki, and it matches, score = 4096
   trim trailing whitespace from extra data
   if flag 0x20,
     return score is random based on ???
   if flag 0x08,
     if title exact match
       return 100
     else
       return 0
   if flag 0x04
     if MatchThemeString(title)
       return 100
   if flag 0x01,
     if MatchThemeString(actor) || MatchThemeString(guest)
       return 99
   if flag 0x02,
     if MatchThemeString(director)
       return 98
   if flag 0x10
     if MatchThemeString(episode) || MatchThemeString(desc)
       return 97
   return 0
*/
struct replay_theme {               /* Corresponds to an RTV ReplayTheme */
/*  0 */ u32 flags;                 /* see theme_flags_bitmapping or above */
/*  4 */ u32 suzuki_id;
/*  8 */ u32 minutes;
/* 12 */ u8  keywords[52];          /* only 48 bytes copied from ReplayTheme0*/
}; /* 64 */

struct replay_channel {              /* corresponds to an RTV ReplayChannel */
/*  0 */ u32 channel_type;           /* see replay_channel_type_mapping */
/*  4 */ u32 quality;                /* see quality_mapping */
/*  8 */ u64 space_allocated;
/* 16 */ u32 keep;                   /* # of episodes to keep */
/* 20 */ u32 stored;                 /* # of episodes currently stored */
/* 24 */ u8  days_of_week;           /* see day_of_week_bitmapping */
/* 25 */ u8  after_padding;
/* 26 */ u8  before_padding;
/* 27 */ u8  flags;                  /* see channel_flags_bitmapping */
/* 28 */ u32 category;               /* bitmask, but only 1 bit set */
/* 32 */ u32 channel_id;             /* primary key, also created time */
/* 36 */ u32 unknown1;
/* 40 */ u32 unknown2;
/* 44 */ u32 unknown3;
/* 48 */ u32 unknown4;
/* 52 */ u32 unknown5;
/* 56 */ u32 unknown6;
/* 60 */ u32 unknown7;
/* 64 */ u32 seconds_reserved;
/* 68 */ u8  label[48];         /* AddressBookEntry, for a received channel */
/*116 */ struct replay_show replay_show;
/*560 */ struct replay_theme replay_theme;
}; /* 624 */

struct replay_guide_snapshot {
    struct guide_header     guide_header;
    struct group_data       group_data;
    struct replay_channel * replay_channels;
    struct replay_show *    replay_shows;
    unsigned int num_shows;
    int stage;
};
    

int parse_guide_header(unsigned char ** pp, struct guide_header * h);
int parse_group_data(unsigned char ** pp, struct group_data * gd);
int parse_channel(unsigned char **pp, struct channel * c);
int parse_replay_show(unsigned char **pp, struct replay_show * rs);
int parse_replay_theme(unsigned char **pp, struct replay_theme * rt);
int parse_replay_channel(unsigned char **pp, struct replay_channel * rc);
int parse_replay_guide_snapshot(unsigned char **pp, struct replay_guide_snapshot * ss);
int parse_replay_guide_snapshot_staged(unsigned char **pp, struct replay_guide_snapshot * ss);

void free_replay_guide_snapshot(struct replay_guide_snapshot * ss);

void dump_guide_header(struct guide_header * h);
void dump_group_data(struct group_data * gd);
void dump_channel(struct channel * c);
void dump_replay_show(struct replay_show * rs);
void dump_replay_theme(struct replay_theme * rt);
void dump_replay_channel(struct replay_channel * rc, struct mapping * category_bitmapping);
void dump_replay_guide_snapshot(struct replay_guide_snapshot * ss);

#endif
