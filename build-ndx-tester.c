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
#include "rtv.h"

#define GOP_START   0x000001b8
#define VIDEO_START 0x000001e0
#define AUDIO_START 0x000001c0

static void report(int recno, u64 video, u64 audio)
{
    u64 block        = video / 0x8000;
    u16 video_offset = video - block * 0x8000;
    u32 audio_offset = audio - video;
    
    printf("    %5d %4x  %4lx 0x%08llx             %08llx %08llx\n",
           recno,
           video_offset,
           audio_offset,
           block * 0x8000,
           video,
           audio);
}

static void write_header(void)
{
    printf("    Recno e0of  c00f  File Offs             e0 pos   c0 pos\n");
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
