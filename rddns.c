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

#include "rddnsclient.h"
#include <stdio.h>
#include <string.h>

static void usage(const char * error)
{
    if (error)
        fprintf(stderr, "ERROR: %s\n", error);
    fprintf(stderr,
            "Usage: rddns lookup <ISN>\n"
            "       rddns update <ISN> <port>\n"
            "\n"
            "ISN format: 00055-09166-55000\n");

    if (error)
        exit(1);
    exit(0);
}

static int lookup(int argc, char ** argv)
{
    unsigned char buf[1024];
    int r;
    
    if (argc != 2)
        usage("Need to specify the INS");
    
    r = rddns_lookup(argv[1], buf, sizeof buf);
    printf("%s\n", buf);

    return r;
}

static int update(int argc, char ** argv)
{
    unsigned char buf[1024];
    int r;
    
    if (argc != 3)
        usage("Need to specify the INS and port");
    
    r = rddns_update(argv[1], atoi(argv[2]), buf, sizeof buf);
    printf("%s\n", buf);

    return r;
}

int main(int argc, char ** argv)
{
    int r = -1;

    if (argc < 2)
        usage(NULL);

    if (strcmp(argv[1], "lookup") == 0)
        r = lookup(argc - 1, argv + 1);
    else if (strcmp(argv[1], "update") == 0)
        r = update(argc - 1, argv + 1);
    else
        usage("Invalid command");

    return r;
}
