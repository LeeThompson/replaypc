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
#include <string.h>
#include "rtv.h"
#include "ndx.h"

#define GOP_START   0x000001b8
#define VIDEO_START 0x000001e0
#define AUDIO_START 0x000001c0

static void report(int recno, u64 video, u64 audio)
{
    struct ndx_record r;
    
    u64 block        = video / 0x8000;
    u16 video_offset = (u16)(video - (block * 0x8000));
    u32 audio_offset = (u32)(audio - video);

    r.flag_1            = 0;
    r.commercial_flag   = 0;
    r.video_offset      = video_offset;
    r.unk_fe            = 0xfe;
    r.macrovision       = 0;
    r.macrovision_count = 0;
    r.audio_offset      = audio_offset;
    r.unused1           = 0;
    r.timestamp         = (u64)(recno * 1000000000.0/2);
    r.stream_position   = block * 0x8000;

    write_ndx_record(stdout, &r);
}

static void write_header(void)
{
    struct ndx_record r;

    memset(&r, 0, sizeof r);
    r.flag_1 = 2;
    r.commercial_flag = 2;
    write_ndx_record(stdout, &r);
}

int main(int argc, char ** argv)
{
    unsigned char buf[32768];
    unsigned char * p;
    size_t words_in_buf;
    size_t cur_word;
    u64 buf_start = 0;
    u64 value;
    u64 video_start = 0, gop = 0, audio_start = 0;
    int recno = 0;

    (void)argc;
    (void)argv;

    write_header();
    
    while ((words_in_buf = fread(buf, 4, sizeof(buf)/4, stdin)) > 0) {
        p = buf;
        for (cur_word = 0; cur_word < words_in_buf; cur_word++) {
            value = rtv_to_u32(&p);
            if (value == VIDEO_START && !gop) {
                video_start = buf_start + cur_word * 4;
            }
            if (value == GOP_START) {
                gop = buf_start + cur_word * 4;
            }
            if (value == AUDIO_START && gop) {
                audio_start = buf_start + cur_word * 4;
                report(recno++, video_start, audio_start);
                video_start = gop = audio_start = 0;
            }
        }
        buf_start += sizeof(buf);
    }
    return 0;
}
