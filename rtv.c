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

#include "rtv.h"

#include <string.h>
#include <stdlib.h>

struct replaytv_version replaytv_version = {4, 3, 0, 230};

u16 rtv_to_u16(unsigned char ** pp)
{
    unsigned char * p;
    u16             r;

    p   = *pp;
    r   = *p++;
    r <<= 8;
    r  |= *p++;

    *pp = p;
    return r;
}

u32 rtv_to_u32(unsigned char ** pp)
{
    unsigned char * p;
    u32             r;

    p   = *pp;
    r   = rtv_to_u16(&p);
    r <<= 16;
    r  |= rtv_to_u16(&p);

    *pp = p;
    return r;
}

u64 rtv_to_u64(unsigned char ** pp)
{
    unsigned char * p;
    u64             r;

    p   = *pp;
    r   = rtv_to_u32(&p);
    r <<= 32;
    r  |= rtv_to_u32(&p);

    *pp = p;
    return r;
}

void rtv_from_u16(unsigned char ** pp, u16 v)
{
    unsigned char * p;

    p = *pp;

    *p++ = (v & 0xff00) >> 8;
    *p++ =  v & 0x00ff;

    *pp = p;
}

void rtv_from_u32(unsigned char ** pp, u32 v)
{
    unsigned char * p;

    p = *pp;

    rtv_from_u16(&p, (u16)((v & 0xffff0000) >> 16));
    rtv_from_u16(&p, (u16)(v & 0x0000ffff));

    *pp = p;
}

void rtv_from_u64(unsigned char ** pp, u64 v)
{
    unsigned char * p;

    p = *pp;

    rtv_from_u32(&p, (u32)((v & 0xffffffff00000000) >> 32));
    rtv_from_u32(&p, (u32)(v & 0x00000000ffffffff));

    *pp = p;
}

/* supports formats:
 * 520411140
 * 4.1
 * 4.1(140)
 * 4.1 (140)
 * 4.1.1
 * 4.1.1.140
 * 4.1.1(140)
 * 4.1.1 (140)
 */
extern int rtv_set_replaytv_version(char * version)
{
    if (strncmp(version, "520", 3) == 0 &&
        strlen(version) == 9) {
        replaytv_version.major = version[3] - '0';
        replaytv_version.minor = version[4] - '0';
        replaytv_version.patch = version[5] - '0';
        replaytv_version.build = atoi(version+6);
        return 0;
    }
    replaytv_version.major = strtoul(version, &version, 10);
    if (version == NULL || *version != '.')
        return -1;              /* need at least major&minor */
    version++;
    replaytv_version.minor = strtoul(version, &version, 10);
    switch (*version) {
        case '\0':              /* '4.1' style */
            replaytv_version.patch = 0;
            replaytv_version.build = 0;
            return 0;
        case '.':               /* '4.1.1'... style */
            version++;
            replaytv_version.patch = strtoul(version, &version, 10);
            break;
        case ' ':               /* '4.1 (140)' */
        case '(':               /* '4.1(140)' */
            replaytv_version.patch = 0;
            break;
        default:
            return -1;
    }
    if (*version == '\0') {     /* '4.1.1' */
        replaytv_version.build = 0;
        return 0;
    }
    switch (*version) {
        case '.':               /* '4.1.1.140' */
            version++;
            break;
        case ' ':               /* '4.1 (140)' or '4.1.1 (140)' */
            version++;
            if (*version != '(')
                return -1;
            version++;
            break;
        case '(':               /* '4.1(140)' or '4.1.1(140)' */
            version++;
            break;
        default:
            return -1;
    }
    replaytv_version.build = strtoul(version, &version, 10);
    if (*version == '\0')
        return 0;
    if (*version == ')')
        return 0;
    return -1;
}

int rtv_split_lines(char * src, char *** pdst)
{
    int num_lines, i;
    char * p;
    char ** dst;

    num_lines = 0;
    p = src;
    while (p) {
        p = strchr(p, '\n');
        if (p) {
            p++;
            num_lines++;
        }
    }

    dst = calloc(num_lines, sizeof(char *));
    dst[0] = src;
    p = src;
    i = 1;
    while (p) {
        p = strchr(p, '\n');
        if (p) {
            *p = '\0';
            p++;
            dst[i] = p;
            i++;
        }
    }

    *pdst = dst;

    return num_lines;
}

void rtv_free_lines(int num_lines, char ** lines) 
{
    (void)num_lines;            /* not used with the above implementation */
    
    free(lines);
}
