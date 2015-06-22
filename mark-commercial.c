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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __unix__
#  include <unistd.h>
#endif

#ifdef WIN32
#  include "gnu_getopt\getopt.h"
#endif

#include "rtv.h"
#include "ndx.h"

static int handle(FILE * in, FILE * out, u32 start, u32 end, int add, int flags)
{
    struct ndx_record this;
    u64 basetime = 0;
    double seconds;
    u32 recno = 0;
 
    while (read_ndx_record(in, &this) > 0) {
        switch (recno) {
            case 0: /* header, passthrough */
                break;
            case 1: /* need to grab the base of the timestamp */
                basetime = this.timestamp;
                /* fallthrough */
            default:
                seconds = ((s64)(this.timestamp - basetime))/1000000000.0;

                if (seconds >= start && seconds <= end) {
                    if (add)
                        this.commercial_flag |= flags;
                    else
                        this.commercial_flag &= ~flags;
                }
                break;
        }
        write_ndx_record(out, &this);
        recno++;
    }
    return 0;
}

static void usage(const char * str)
{
    if (str)
        fprintf(stderr, "*** ERROR: %s\n\n", str);

    fprintf(stderr,
"Usage: commercial [-i <i>] [-o <o>] [-s <s>] [-e <e>] [-r] [filename]\n"
"       -i -- input filename\n"
"       -o -- output filename\n"
"       -s -- start time, in seconds\n"
"       -e -- end time, in seconds\n"
"       -r -- remove rather than add\n"
"\n"
"If no files are given, reads from stdin and writes to stdout\n"
"If [filename] is given, it's used for both input and output\n"
"If -r is given with no start or end time, all commercial breaks are removed\n"
"If -r is not given, start and end time must be given\n"
        );
    exit(str ? 1 : 0);
}

static void error(const char * str)
{
    fprintf(stderr, "*** ERROR: %s\n", str);
    exit(1);
}

int main(int argc, char ** argv)
{
    FILE * in        = NULL,
         * out       = NULL;
    u32    start     = 0,
           end       = 0;
    int    start_set = 0,
           end_set   = 0,
           add       = 1,
           flags     = 0x01,
           ch;
    char * p;
    
    while ((ch = getopt(argc, argv, "i:o:s:e:rhf:")) != EOF) {
        switch(ch) {
            case 'i':
                if (in)
                    usage("Can only specify input once");
                in = fopen(optarg, "r");
                if (!in)
                    error("Couldn't open input file");
                break;
            case 'o':
                if (out)
                    usage("Can only specify output once");
                out = fopen(optarg, "w");
                if (!out)
                    error("Couldn't open output file");
                break;
            case 's':
                if (start_set)
                    usage("Can only specify one start time");
                start = strtoul(optarg, &p, 0);
                if (*p != 0)
                    usage("Start time must be numeric");
                start_set = 1;
                break;
            case 'e':
                if (end_set)
                    usage("Can only specify one end time");
                end = strtoul(optarg, &p, 0);
                if (*p != 0)
                    usage("End time must be numeric");
                end_set = 1;
                break;
            case 'r':
                if (!add)
                    usage("Only use -r once");
                add = 0;
                break;
            case 'h':
                usage(NULL);
                break;
            case 'f':
                flags = strtoul(optarg, NULL, 0);
                break;
            default:
                usage("");
        }
    }
    if (argc - optind > 1)
        usage("If input and output are separate files, must use -i and -o");
    if (argc - optind == 0) {
        if (!in)
            in = stdin;
        if (!out)
            out = stdout;
    }
    if (argc - optind == 1) {
        if (in || out)
            usage("If you use -i or -o, use both or std{in,out}");
        in = fopen(argv[optind], "r");
        if (!in)
            error("Couldn't open file to read");
        out = fopen(argv[optind], "r+");
        if (!out)
            error("Couldn't open file to write");
    }

    if ((start_set && !end_set) || (end_set && !start_set))
        usage("If you specify start or end, specify both");
    if (flags == 0x01 && add && !start_set && !end_set)
        usage("It doesn't make much sense to set the whole stream to commercial");
    if (!start_set)
        start = 0;
    if (!end_set)
        end = 0xffffffff;

    return handle(in, out, start, end, add, flags);
}
