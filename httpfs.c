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
#include "httpfsclient.h"
#include "bigfile.h"

#ifdef WIN32
#include "gnu_getopt\getopt.h"
#endif

#ifdef __unix__
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if 0
#include <sys/types.h>
#include <sys/wait.h>
#endif

#define MIN(x,y) ((x)<(y) ? (x) : (y))

static void usage(const char * err)
{
    if (err)
        fprintf(stderr, "*** ERROR: %s\n\n", err);

    fprintf(stderr,
"Usage: httpfs [-a address] [-s software_version] [command <args...>]\n"
"Commands:\n"
"  ls [-l] [path]\n"
"  fstat <name>\n"
"  volinfo <name>\n"
"  cp <src> <dst>\n"
"  mv <from> <to>\n"
"  rm <name>\n"
"  create <name>\n"
"  mkdir <name>\n"
"  readfile [-d <msec>] [-f <dst>] [-s <size>] [-p <pos>] [-v <level>] <name>\n"
"  writefile [-f <src>|-s <size>] [-p <pos>] [-v <level>]<name>\n"
"\n"
"Verboseness levels (only implemented for readfile, currently):\n"
"     0: no extraneous output\n"
"     1: succinct status information suitable for parsing, but nothing that\n"
"        requires optional network traffic\n"
"     2: like 1, but adds information that may require additional network\n"
"        traffic\n"
"     3: pretty-printed status graphs for humans to look at (default)\n"
"\n"
"Address defaults to $RTV4K_ADDRESS, if that's set; if not, -a must be used\n"
"Software version defaults to 4.3.0 (230); can be set with $RTV4K_VERSION\n"
"or -s; format should be like '520411140' or '4.1.1.140'\n"
        );

    exit(err ? 1 : 0);
}

static void print_status_bar(FILE * fp, u64 done, u64 total) 
{
    int   percent;
    char  buf[100];
    int   i;
    char *p;
    
    percent = (done*100)/total;
    p = buf;
    *p++ = '[';
    for (i = 0; i < 50; i++) {
        if (i*2 < percent)
            *p++ = '#';
        else
            *p++ = ' ';
    }
    *p++ = ']';
    sprintf(p, "  %"U64F"d/%"U64F"d (%d%%)\r", done, total, percent);
    fprintf(fp, "%s", buf);
}
    

static int hfs_cp(const char * address, int argc, char ** argv)
{
    char * data = NULL;
    unsigned long status;

    if (argc != 3)
        usage("cp needs two arguments");
    
    status = hfs_do_simple(&data, address,
                           "cp",
                           "src", argv[1],
                           "dest", argv[2],
                           NULL);
    if (status != 0)
        fprintf(stderr, "Error %ld\n", status);
    free(data);
    return status;
}

static int hfs_create(const char * address, int argc, char ** argv)
{
    char * data = NULL;
    unsigned long status;

    if (argc != 2)
        usage("create needs one argument");
    
    status = hfs_do_simple(&data, address,
                           "create",
                           "name", argv[1],
                           NULL);
    if (status != 0)
        fprintf(stderr, "Error %ld\n", status);
    free(data);
    return status;
}

static int hfs_fstat(const char * address, int argc, char ** argv)
{
    char * data = NULL;
    unsigned long status;

    if (argc != 2)
        usage("fstat needs one argument");
    
    status = hfs_do_simple(&data, address,
                           "fstat",
                           "name", argv[1],
                           NULL);
    if (status != 0)
        fprintf(stderr, "Error %ld\n", status);
    printf("%s", data);
    free(data);
    return status;
}

static int vstrcmp(const void * v1, const void * v2)
{
    return (strcmp(*(char **)v1, *(char **)v2));
}

static const char * format_type(char type) 
{
    switch(type) {
        case 'f': return "-";
        case 'd': return "d";
        default:  return "?";
    }
}

static char * format_perm(char * perm) 
{
    static char rtn[4];
    char * p;

    strcpy(rtn, "-- ");

    if (strlen(perm) > 3) {
        fprintf(stderr, "ERROR: format_perm: unexpectedly long permission string :%s:\n", perm);
        strcpy(rtn, "ERR");
        return rtn;
    }
    
    for (p = perm; *p; p++) {
        switch(*p) {
            case 'r':
                rtn[0] = 'r';
                break;
            case 'w':
                rtn[1] = 'w';
                break;
            default:
                rtn[2] = *p;
                break;
        }
    }
    return rtn;
}

static char * format_time(u64 ttk) 
{
    static char results[256];
    
    time_t tt;
    int msec;
    struct tm * tm;
    
    tt = ttk / 1000;
    msec = ttk % 1000;

    tm = localtime(&tt);

    strftime(results, sizeof results, "%Y-%m-%d %H:%M:%S", tm);

    sprintf(results + 19, ".%03d", msec);

    return results;
}

static int ls_l(const char * address, const char * dir, const char * filename)
{
    char          pathname[256];
    unsigned long status;
    char *        data;
    char **       lines;
    int           num_lines;
    int           i;
    char          type = '?';
    char *        perm = (char *)"";
    u64           size = 0;
    u64           filetime = 0;
    
    if (strlen(dir) + strlen(filename) + 2 > sizeof pathname) {
        fprintf(stderr, "ERROR: ls_l: dir + filename too long: %s/%s\n",
                dir, filename);
        return -1;
    }
    
    sprintf(pathname, "%s/%s", dir, filename);
    status = hfs_do_simple(&data, address,
                           "fstat",
                           "name", pathname,
                           NULL);
    if (status != 0) {
        fprintf(stderr, "ls_l %s: Error %ld\n", filename, status);
        return status;
    }

    /* should have a higher-level fstat func that does the request
       and parses it.  or at least a common fstat parser */
    num_lines = rtv_split_lines(data, &lines);
    for (i = 0; i < num_lines; i++) {
        if (strncmp(lines[i], "type=", 5) == 0) {
            type = lines[i][5];
        } else if (strncmp(lines[i], "size=", 5) == 0) {
            sscanf(lines[i]+5, "%"U64F"d", &size);
        } else if (strncmp(lines[i], "ctime=", 6) == 0) {
            sscanf(lines[i]+6, "%"U64F"d", &filetime);
        } else if (strncmp(lines[i], "perm=", 5) == 0) {
            perm = lines[i] + 5;
        }
    }
    
    printf("%s%s\t%12"U64F"d %s %s\n",
           format_type(type),
           format_perm(perm),
           size,
           format_time(filetime),
           filename);

    rtv_free_lines(num_lines, lines);
    free(data);

    return 0;
}

static int hfs_ls(const char * address, int argc, char ** argv)
{
    char *        dir;
    char *        data;
    char **       files;
    int           ch;
    int           i;
    int           num_files;
    int           opt_l = 0;
    unsigned long status;
    
    while ((ch = getopt(argc, argv, "l")) != EOF) {
        switch(ch) {
            case 'l':
                opt_l = 1;
                break;
            default:
                usage("Invalid argument to ls");
        }
    }

    if (optind < argc) {
        if (opt_l) {
            status = hfs_do_simple(&data, address,
                                   "fstat",
                                   "name", argv[optind],
                                   NULL);
            if (status == 0 && strncmp(data, "type=f", 6) == 0) {
                free(data);
                ls_l(address, "", argv[optind]);
                return 0;
            }
            free(data);
        }
        dir = argv[optind];
    } else {
        dir = (char *)"/";
    }

        
    status = hfs_do_simple(&data, address,
                           "ls",
                           "name", dir,
                           NULL);
    if (status != 0) {
        fprintf(stderr, "Error %ld\n", status);
        return status;
    }

    num_files = rtv_split_lines(data, &files);
    qsort(files, num_files, sizeof(char *), vstrcmp);
    for (i = 0; i < num_files; i++) {
        if (opt_l) {
            ls_l(address, dir, files[i]);
        } else {
            fprintf(stdout, "%s\n", files[i]);
        }
    }

    rtv_free_lines(num_files, files);
    free(data);
    return 0;
}

static int hfs_mkdir(const char * address, int argc, char ** argv)
{
    char * data = NULL;
    unsigned long status;

    if (argc != 2)
        usage("mkdir needs one argument");
    
    status = hfs_do_simple(&data, address,
                           "mkdir",
                           "name", argv[1],
                           NULL);
    if (status != 0)
        fprintf(stderr, "Error %ld\n", status);
    free(data);
    return status;
}

static int hfs_mv(const char * address, int argc, char ** argv)
{
    char * data = NULL;
    unsigned long status;

    if (argc != 3)
        usage("mv needs two arguments");
    
    status = hfs_do_simple(&data, address,
                           "mv",
                           "old", argv[1],
                           "new", argv[2],
                           NULL);
    if (status != 0)
        fprintf(stderr, "Error %ld\n", status);
    free(data);
    return status;
}

static u64 get_filesize(const char * address, const char * name)
{
    char * data = NULL;
    char ** lines;
    int num_lines;
    int i;
    u64 size;
    unsigned long status;

    status = hfs_do_simple(&data, address,
                           "fstat",
                           "name", name,
                           NULL);
    if (status != 0) {
        fprintf(stderr, "Error fstat %ld\n", status);
        free(data);
        return -1;
    }

    num_lines = rtv_split_lines(data, &lines);
    for (i = 0; i < num_lines; i++) {
        if (strncmp(lines[i], "size=", 5) == 0) {
            sscanf(lines[i]+5, "%"U64F"d", &size);
            rtv_free_lines(num_lines, lines);
            free(data);
            return size;
        }
    }
    rtv_free_lines(num_lines, lines);
    free(data);
    return -1;
}

struct read_data
{
    BIGFILE * dstfile;
    u64 bytes;
    u64 fullsize;
    int verbose;
};

static void hfs_rf_callback(unsigned char * buf, size_t len, void * vd)
{
    struct read_data * rd = vd;

    bfwrite(buf, len, 1, rd->dstfile);
    rd->bytes += len;
    switch(rd->verbose) {
        case 2:
            fprintf(stderr, "Total: %"U64F"d\n", rd->fullsize);
            rd->verbose--;
            /* fallthrough */
        case 1:
            fprintf(stderr, "Read: %"U64F"d\n", rd->bytes);
            break;
        case 3:
            print_status_bar(stderr, rd->bytes, rd->fullsize);
            break;
    }
}

static int hfs_readfile(const char * address, int argc, char ** argv)
{
    struct read_data data;
    unsigned long status;
    const char * pos  = NULL;
    const char * size = NULL;
    const char * file = NULL;
    const char * name;
    int ch;
    u32 delay = 75;

    memset(&data, 0, sizeof data);
    data.verbose = 3;
    
    while ((ch = getopt(argc, argv, "d:f:p:s:v:")) != EOF) {
        switch(ch) {
            case 'f':
                file = optarg;
                break;
            case 'p':
                pos = optarg;
                break;
            case 's':
                size = optarg;
                break;
            case 'd':
                delay = atoi(optarg);
                break;
            case 'v':
                data.verbose = atoi(optarg);
                break;
            default:
                usage("Invalid argument to readfile");
        }
    }

    if (argc - optind != 1)
        usage("Need to specify exactly one filename for readfile");

    name = argv[optind];
    
    if (!pos)
        pos = "0";

    if (file) {
        data.dstfile = bfopen(file, "w");
        if (!data.dstfile) {
            perror("bfopen dstfile");
            return -1;
        }
    } else {
        data.dstfile = bfreopen(stdout);
    }

    if (data.verbose > 1) {
        data.fullsize = get_filesize(address, name);
    }
    
    status = hfs_do_chunked(hfs_rf_callback, &data, address, delay,
                            "readfile",
                            "pos",  pos,
                            "size", size,
                            "name", name,
                            NULL);
    if (status != 0)
        fprintf(stderr, "Error: %ld\n", status);

    return status;
}

static int hfs_rm(const char * address, int argc, char ** argv)
{
    char * data = NULL;
    unsigned long status;

    if (argc != 2)
        usage("rm needs one arguments");
    
    status = hfs_do_simple(&data, address,
                           "rm",
                           "name", argv[1],
                           NULL);
    if (status != 0)
        fprintf(stderr, "Error %ld\n", status);
    free(data);
    return status;
}

static int hfs_volinfo(const char * address, int argc, char ** argv)
{
    char * data = NULL;
    unsigned long status;

    if (argc != 2)
        usage("volinfo needs one arguments");
    
    status = hfs_do_simple(&data, address,
                           "volinfo",
                           "name", argv[1],
                           NULL);
    if (status != 0)
        fprintf(stderr, "Error %ld\n", status);
    printf("%s", data);
    free(data);
    return status;
}

struct write_data 
{
    BIGFILE * srcfile;
    u64 bytes;
    u64 fullsize;
    int verbose;
};

static int hfs_wf_callback(unsigned char * buf, size_t len, void * vd)
{
    struct write_data * wd = vd;
    int r = 0;
    size_t l;
    u64 bytes_to_go;

    bytes_to_go = wd->fullsize - wd->bytes;
    l = MIN(len, bytes_to_go);
    if (l) {
        r = bfread(buf, l, 1, wd->srcfile);
    }
    wd->bytes += r * l;

    switch(wd->verbose) {
        case 2:
            fprintf(stderr, "Total: %"U64F"d\n", wd->fullsize);
            wd->verbose--;
            /* fallthrough */
        case 1:
            fprintf(stderr, "Written: %"U64F"d\n", wd->bytes);
            break;
        case 3:
            print_status_bar(stderr, wd->bytes, wd->fullsize);
            break;
    }
    
    return r * l;
}

static int hfs_writefile(const char * address, int argc, char ** argv)
{
    struct write_data wd;
    unsigned long status;
    const char * pos = NULL;
    const char * size = NULL;
    const char * name = NULL;
    const char * file = NULL;
    char * data = NULL;
    int ch;
    u64 size_num;

    memset(&data, 0, sizeof data);
    wd.verbose = 3;
    
    while ((ch = getopt(argc, argv, "f:p:s:v:")) != EOF) {
        switch(ch) {
            case 'f':
                file = optarg;
                break;
            case 'p':
                pos = optarg;
                break;
            case 's':
                size = optarg;
                break;
            case 'v':
                wd.verbose = atoi(optarg);
                break;
            default:
                usage("Invalid argument to readfile");
        }
    }

    if (argc - optind != 1)
        usage("Need to specify exactly one filename for writefile");

    name = argv[optind];

    if (!size && !file)
        usage("Either a file or a size must be given for writefile");
    
    if (!pos)
        pos = "0";

    if (file) {
        char sizebuf[32];
        wd.srcfile = bfopen(file, "r");
        if (!wd.srcfile) {
            perror("open srcfile");
            return -1;
        }
        bfseek(wd.srcfile, 0, SEEK_END);
        size_num = bftell(wd.srcfile);
        bfseek(wd.srcfile, 0, SEEK_SET);
        sprintf(sizebuf, "%"U64F"d", size_num);
        size = sizebuf;
    } else {
        wd.srcfile = bfreopen(stdin);
        sscanf(size, "%"U64F"d", &size_num);
    }
    wd.fullsize = size_num;
    wd.bytes = 0;
    
    status = hfs_do_simple(&data, address,
                           "create",
                           "name", name,
                           NULL);
    free(data);
    if (status != 0 && status != RTV_EEXIST)
        fprintf(stderr, "Error create: %ld\n", status);
    
    status = hfs_do_post_simple(&data, address,
                                hfs_wf_callback, &wd, size_num,
                                "writefile",
                                "pos",  pos,
                                "name", name,
                                "size", size,
                                NULL);
    if (status != 0)
        fprintf(stderr, "Error: %ld\n", status);
    free(data);
    return status;
}

static struct {
    const char * command;
    int (*fn)(const char *, int, char **);
} commands[] = {
    { "cp",             hfs_cp          },
    { "create",         hfs_create      },
    { "fstat",          hfs_fstat       },
    { "ls",             hfs_ls          },
    { "mkdir",          hfs_mkdir       },
    { "mv",             hfs_mv          },
    { "readfile",       hfs_readfile    },
    { "rm",             hfs_rm          },
    { "volinfo",        hfs_volinfo     },
    { "writefile",      hfs_writefile   }
};

int main(int argc, char ** argv)
{
    char * address;
    char * version;
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
            case 's':
                version = optarg;
                break;
            default:
                usage("Invalid argument");
        }
    }
    if (optind == argc || address == NULL)
        usage(NULL);

    if (version)
        if (rtv_set_replaytv_version(version) != 0)
            usage("Invalid software version");

    my_optind = optind;
    optind    = 1;
    
    for (i = 0; i < num_commands; i++) {
        if (strcmp(argv[my_optind], commands[i].command) == 0) {
            r = commands[i].fn(address, argc - my_optind, argv + my_optind);
            break;
        }
    }
    if (i == num_commands) {
        usage("Invalid command");
    }

    return r;
}
