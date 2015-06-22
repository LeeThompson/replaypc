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

static int handle(FILE * in, FILE * out)
{
    struct ndx_record this;

    /* header */
    if (read_ndx_record(in, &this) <= 0) {
        fprintf(stderr, "ERROR: couldn't read header\n");
        return -1;
    }
    if (this.flag_1 != 2 || this.commercial_flag != 2) {
        fprintf(stderr, "ERROR: invalid header\n");
        return -1;
    }
    this.video_offset = 0;      /* in header, part of that is really a flag */
    write_ndx_record(out, &this);
    
    while (read_ndx_record(in, &this) > 0) {
        this.unk_fe = 0xfe;
        this.macrovision = 0;
        this.macrovision_count = 0;
        write_ndx_record(out, &this);
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
"\n"
"If no files are given, reads from stdin and writes to stdout\n"
"If [filename] is given, it's used for both input and output\n"
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
    int    ch;
    
    while ((ch = getopt(argc, argv, "i:o:h")) != EOF) {
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
            case 'h':
                usage(NULL);
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

    return handle(in, out);
}
