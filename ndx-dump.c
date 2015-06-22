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

#include "ndx.h"
#include "bigfile.h"
#include <string.h>
#include <stdlib.h>

static void unexpected(int recno, const char * reason, u64 value)
{
    fprintf(stderr, "UNEXPECTED %d: %s; value == %"U64F"x\n",
            recno, reason, value);
    printf("#UNEXPECTED %d %s; value == %"U64F"x\n", recno, reason, value);
}

static void check_header(struct ndx_record * pr)
{
    if (pr->flag_1 != 0x02)
        unexpected(-1, "Header flag_1 not 0x02", pr->flag_1);
    if (pr->commercial_flag != 0x02)
        unexpected(-1, "Header commercial_flag not 0x02", pr->commercial_flag);
    if (pr->video_offset != 0)
        unexpected(-1, "Header video_offset not 0", pr->video_offset);
    if (pr->unk_fe != 0)
        unexpected(-1, "Header unk_fe not 0", pr->unk_fe);
    if (pr->macrovision != 0)
        unexpected(-1, "Header macrovision not 0", pr->macrovision_count);
    if (pr->macrovision_count != 0)
        unexpected(-1, "Header macrovision_count not 0", pr->macrovision_count);
    if (pr->audio_offset != 0)
        unexpected(-1, "Header audio_offset not 0", pr->audio_offset);
    if (pr->unused1 != 0)
        unexpected(-1, "Header unused1 not 0", pr->unused1);
    if (pr->timestamp != 0)
        unexpected(-1, "Header timestamp not 0", pr->timestamp);
    if (pr->stream_position != 0)
        unexpected(-1, "Header stream_position not 0", pr->stream_position);
}

static char * format_seconds(float seconds)
{
    static char buffer[12];
    int minutes;

    minutes = (int)seconds / 60;
    seconds -= minutes * 60;

    if (minutes)
        sprintf(buffer, "%3d:", minutes);
    else
        strcpy(buffer, "    ");
    sprintf(buffer + 4, "%07.4f", seconds);

    return buffer;
}

static int explore(FILE * ndx, BIGFILE * mpg)
{
    unsigned char buf[1024], *p;
    struct 
    {
        struct ndx_record r;
        float             seconds;
        int               bad_video;
        int               bad_audio;
    } this, last;
    int recno    = 0;
    u64 basetime = 0;
    double commercial_start_seconds = 0;
    int in_commercial = 0;
    
    memset(&this, 0, sizeof this);
    if (read_ndx_record(ndx, &this.r) <= 0) {
        unexpected(-1, "Couldn't read header line", 0);
        return -1;
    }
    check_header(&this.r);
    printf("87654321 SC Recno VidO  AudO File Pos   Time Offs VideoPos AudioPos VAM\n");

    last = this;
    memset(&this, 0, sizeof this);
    while (read_ndx_record(ndx, &this.r) > 0) {
        /* 0x8000 -- unknown
           0x4000 -- unknown, new with jan 30 sw
           0x2000 -- PPV/protected (?? not any more?)
           0x0800 -- unknown, new with jan 30 sw
           0x0400 -- unknown, new with jan 30 sw
           0x0200 -- unknown, new with jan 30 sw
           0x0100 -- unknown, new with jan 30 sw
           0x0002 -- black screen
           0x0001 -- commercial
        */
        if (this.r.commercial_flag & ~0x03)
            unexpected(recno, "commercial flag has unexpected bit set",
                       this.r.commercial_flag);
            
        if (this.r.unk_fe != 0xfe && this.r.macrovision == 0) {
            unexpected(recno, "unk_fe != 0xfe without macrovision set", this.r.unk_fe);
        }
        if (this.r.macrovision != 0 && this.r.macrovision != 3) {
            unexpected(recno, "macrovision != 0 or 3", this.r.macrovision);
        }
        if (this.r.macrovision == 0 && this.r.macrovision_count != 0) {
            unexpected(recno, "macrovision clear but count != 0", this.r.macrovision_count);
        }
        if (this.r.macrovision && this.r.macrovision_count == 0) {
            unexpected(recno, "macrovision count clear but macrovision set", this.r.macrovision);
        }
        if (this.r.unused1 != 0)
            unexpected(recno, "unused1 != 0", this.r.unused1);
        if (this.r.stream_position % 0x8000 != 0)
            unexpected(recno, "stream position not 32k boundary",
                       this.r.stream_position);
        if (this.r.video_offset % 4 != 0)
            unexpected(recno, "video_offset % 4 != 0", this.r.video_offset);
        if (this.r.video_offset >= 0x8000)
            unexpected(recno, "video_offset larger than cluster size",
                       this.r.video_offset);
        if (this.r.audio_offset % 4 != 0)
            unexpected(recno, "audio_offset % 4 != 0", this.r.audio_offset);
            
        if (recno == 0) {
            basetime   = this.r.timestamp;
        }
        this.seconds = ((s64)(this.r.timestamp - basetime)) / 1000000000.0;

        if (mpg) {
            bfseek(mpg, this.r.stream_position + this.r.video_offset,
                   SEEK_SET);
            bfread(buf, 4, 1, mpg);
            p = buf;
            if (rtv_to_u32(&p) != 0x000001e0) {
                this.bad_video = 1;
            } else {
                bfseek(mpg, this.r.stream_position + this.r.video_offset + 56, SEEK_SET);
                bfread(buf, 4, 1, mpg);
                p = buf;
                if (rtv_to_u32(&p) != 0x000001b8)
                    unexpected(recno, "Video offset correct, but GOP not found", this.r.video_offset);
            }
            if (last.bad_video) {
                if ((bfseek(mpg, this.r.stream_position + last.r.video_offset,
                             SEEK_SET)) < 0) {
                    perror("bfseek");
                }
                
                bfread(buf, 4, 1, mpg);
                p = buf;
                if (rtv_to_u32(&p) != 0x000001e0) {
                    p = buf;
                    unexpected(recno, "Video not found after applying race condition correction", rtv_to_u32(&p));
                }
                if (this.r.stream_position - last.r.stream_position > 0x10000)
                    unexpected(recno, "large stream block following bad video offset",
                               this.r.stream_position - last.r.stream_position);            }

            bfseek(mpg, this.r.stream_position + this.r.video_offset + this.r.audio_offset, SEEK_SET);
            bfread(buf, 4, 1, mpg);
            p = buf;
            if (rtv_to_u32(&p) != 0x000001c0) {
                this.bad_audio = 1;
                if (this.bad_audio && !this.bad_video)
                    unexpected(recno, "not audio without being not video", 0);
            }
            if (last.bad_audio) {
                bfseek(mpg, this.r.stream_position + last.r.video_offset + this.r.audio_offset, SEEK_SET);
                bfread(buf, 4, 1, mpg);
                p = buf;
                if (rtv_to_u32(&p) != 0x000001c0) {
                    bfseek(mpg, this.r.stream_position + last.r.video_offset + last.r.audio_offset, SEEK_SET);
                    bfread(buf, 4, 1, mpg);
                    p = buf;
                    if (rtv_to_u32(&p) != 0x000001c0) {
                        p = buf;
                        unexpected(recno, "Audio not found after applying race condition correction", rtv_to_u32(&p));
                    }
                }
            }
        }
        
        p = buf;
        if (this.r.commercial_flag & 0x1) {
            if (!in_commercial) {
                commercial_start_seconds = this.seconds;
                if (!(this.r.commercial_flag & 0x02))
		    unexpected(recno, "Start of commercial without 0x02 flag",
		  	       this.r.commercial_flag);
                in_commercial = 1;
            }
        } else if (in_commercial) {
            printf("# Commercial %s",format_seconds(commercial_start_seconds));
            printf(" - %s", format_seconds(this.seconds));
            printf(" (%s)\n",
                   format_seconds(this.seconds - commercial_start_seconds));
            in_commercial = 0;
        }
            
        p += sprintf(p, "%c%c%c%c%c%c%c%c %c%c %5d %4x %5lx %08Lx %s %08Lx %08Lx %c%c%c",
                     this.r.flag_1 & 0x80 ? 'X' : '.',
                     this.r.flag_1 & 0x40 ? 'X' : '.',
                     this.r.flag_1 & 0x20 ? 'X' : '.',
                     this.r.flag_1 & 0x10 ? 'X' : '.',
                     this.r.flag_1 & 0x08 ? 'X' : '.',
                     this.r.flag_1 & 0x04 ? 'X' : '.',
                     this.r.flag_1 & 0x02 ? 'X' : '.',
                     this.r.flag_1 & 0x01 ? 'X' : '.',
                     this.r.commercial_flag & 0x02 ? 'X' : '.',
                     this.r.commercial_flag & 0x01 ? 'X' : '.',
                     recno,
                     this.r.video_offset,
                     this.r.audio_offset,
                     this.r.stream_position,
                     format_seconds(this.seconds),
                     this.r.stream_position + this.r.video_offset,
                     this.r.stream_position + this.r.video_offset + this.r.audio_offset,
                     this.bad_video ? last.bad_video ? 'O' : 'o' : '.',
                     this.bad_audio ? last.bad_audio ? 'O' : 'o' : '.',
                     this.r.macrovision ?              'O'       : '.'
            );
        puts(buf);
        recno++;
        last = this;
        memset(&this, 0, sizeof this);
    }
    return 0;
}

int main(int argc, char ** argv)
{
    char * basefile;
    FILE * ndx;
    BIGFILE * mpg;

    if (argc > 1) {
        char filename[1024];
        basefile = argv[1];
        sprintf(filename, "%s.ndx", basefile);
        ndx = fopen(filename, "r");
        if (!ndx) {
            fprintf(stderr, "Couldn't open %s, aborting\n", filename);
            exit(1);
        }
        sprintf(filename, "%s.mpg", basefile);
        mpg = bfopen(filename, "r");
        if (!mpg) {
            fprintf(stderr, "Couldn't open %s, reducing checking\n", filename);
        }
    } else {
        fprintf(stderr, "No filename given, just doing ndx checking\n");
        ndx = stdin;
        mpg = NULL;
    }
    
    return explore(ndx, mpg);
}
