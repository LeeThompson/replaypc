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

#ifndef RTV_H
#define RTV_H

#ifdef __unix__
#define _LARGEFILE64_SOURCE
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned long      u32;
typedef unsigned long long u64;
typedef          long long s64;
#endif

#ifdef _WIN32
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned long      u32;
typedef unsigned __int64   u64;
typedef          __int64   s64;
#endif

#ifdef __APPLE__
#define U64F "q"
#else
#define U64F "ll"
#endif

#ifndef EXPECT_CAN_EXIT
#define EXPECT_CAN_EXIT 0
#endif

struct replaytv_version {
    int major, minor, patch, build;
};

extern struct replaytv_version replaytv_version;

extern u16  rtv_to_u16(unsigned char ** pp);
extern u32  rtv_to_u32(unsigned char ** pp);
extern u64  rtv_to_u64(unsigned char ** pp);
extern void rtv_from_u16(unsigned char ** pp, u16 v);
extern void rtv_from_u32(unsigned char ** pp, u32 v);
extern void rtv_from_u64(unsigned char ** pp, u64 v);

extern int rtv_set_replaytv_version(char * version);

extern int rtv_split_lines(char * src, char *** dst);
extern void rtv_free_lines(int num, char ** lines);

#define expect(x) do {                                          \
    if (!(x)) {                                                 \
        fprintf(stderr, "Unexpected %s %s:%d\n", #x, __FILE__, __LINE__);    \
        if (EXPECT_CAN_EXIT) (exit(-1));                        \
    }                                                           \
} while(0)

#endif
