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

#include <assert.h>
#include <string.h>
#include "rtv.h"
#include "ndx.h"

int read_ndx_record(FILE * fp, struct ndx_record * pr)
{
    unsigned char buf[32], * p;
    int r;
    
    if ((r = fread(buf, 1, 32, fp)) <= 0)
        return r;
    
    p = buf;
    memset(pr, 0, 32);
        
    pr->flag_1            = *p++;
    pr->commercial_flag   = *p++;
    pr->video_offset      = rtv_to_u16(&p);
    pr->unk_fe            = *p++;
    pr->macrovision       = *p++;
    pr->macrovision_count = rtv_to_u16(&p);
    pr->audio_offset      = rtv_to_u32(&p);
    pr->unused1           = rtv_to_u32(&p);
    pr->timestamp         = rtv_to_u64(&p);
    pr->stream_position   = rtv_to_u64(&p);

    assert(p == buf + 32);
    return 1;
}

int write_ndx_record(FILE * fp, struct ndx_record * pr)
{
    unsigned char buf[32], * p;

    p = buf;
    *p++ = pr->flag_1;
    *p++ = pr->commercial_flag;
    rtv_from_u16(&p, pr->video_offset);
    *p++ = pr->unk_fe;
    *p++ = pr->macrovision;
    rtv_from_u16(&p, pr->macrovision_count);
    rtv_from_u32(&p, pr->audio_offset);
    rtv_from_u32(&p, pr->unused1);
    rtv_from_u64(&p, pr->timestamp);
    rtv_from_u64(&p, pr->stream_position);

    assert(p == buf + 32);
    return fwrite(buf, 1, 32, fp);
}
