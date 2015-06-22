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
#include "guideclient.h"

#ifdef WIN32
#include "gnu_getopt\getopt.h"
#endif

#ifdef __unix__
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char * err)
{
    if (err)
        fprintf(stderr, "*** ERROR: %s\n\n", err);

    fprintf(stderr,
"Usage: rtvguide [-a address] [-s software version] [command <args...>]\n"
"Commands:  get [ -f <dstfile> ] [ -a ] [ -t <timestamp> ]\n"
"               -a -- auto mode: name file \"snapshot.<address>.<timestamp>\"\n"
"                     and output timestamp on stdout; if -t is also given, and\n"
"                     the file hasn't changed, don't modify it.\n"
"               -t -- if a timestamp is given, only output if the data has\n"
"                     changed.\n\n"
"Address defaults to $RTV4K_ADDRESS, if that's set; if not, -a must be used\n"
"Software version defaults to 4.3.0 (230); can be set with $RTV4K_VERSION\n"
"or -s; format should be like '520411140' or '4.1.1.140'\n"

        );
    exit(err ? 1 : 0);
}

static int guide_get(const char * address, int argc, char ** argv)
{
    int             ch;
    int             auto_mode      = 0;
    char *          orig_timestamp = "0";
    char *          filename       = NULL;
    unsigned char * result, * timestamp;
    unsigned long   size;
    unsigned long   status;
    FILE *          fp;
    
    while ((ch = getopt(argc, argv, "f:at:")) != EOF) {
        switch(ch) {
            case 'f':
                filename = optarg;
                break;
            case 'a':
                auto_mode = 1;
                break;
            case 't':
                orig_timestamp = optarg;
                break;
            default:
                usage("Invalid argument to guide_get");
        }
    }

    if (optind != argc) {
        usage("Invalid argument to guide_get");
    }
    if (auto_mode && filename) {
        usage("Auto mode and explicit filename are incompatible.");
    }
    
    status = guide_client_get_snapshot(&result, &timestamp, &size,
                                  address, orig_timestamp);
    if (status != 0) {
        fprintf(stderr, "Error get_snapshot %ld\n", status);
        return -1;
    }

    if (strcmp(timestamp, "upToDate") == 0)
        return 0;

    if (auto_mode) {
        filename = malloc(64);
        sprintf(filename, "snapshot.%s.%s", address, timestamp);
    }
    if (filename) {
        fp = fopen(filename, "w");
        if (!fp) {
            perror("Open output file");
            return -1;
        }
    } else {
        fp = stdout;
    }

    fwrite(result, size, 1, fp);

    if (auto_mode) {
        printf("%s\n", timestamp);
    }

    return 0;
}


static struct 
{
    const char * command;
    int (*fn)(const char *, int, char **);
} commands[] = {
    { "get",      guide_get         },
};

int main(int argc, char ** argv)
{
    char * address, * version;
    int    num_commands = sizeof(commands)/sizeof(commands[0]);
    int    ch;
    int    r            = -1;
    int    i;
    int    my_optind;

    address = getenv("RTV4K_ADDRESS");
    version = getenv("RTV4K_VERSION");
    
    while ((ch = getopt(argc, argv, "+a:s:")) != EOF) {
        switch(ch) {
            case 'a':
                address = optarg;
                break;
            default:
                usage("Invalid argument");
        }
    }
    if (optind == argc || address == NULL)
        usage(NULL);

    my_optind = optind;
    optind    = 1;

    if (version)
        if (rtv_set_replaytv_version(version) != 0)
            usage("Invalid software version");

    for (i = 0; i < num_commands; i++) {
        if (strcmp(argv[my_optind], commands[i].command) == 0) {
            r = commands[i].fn(address, argc - my_optind, argv + my_optind);
            break;
        }
    }
    if (i == num_commands) {
        usage("INvalid command");
    }

    return r;
}
