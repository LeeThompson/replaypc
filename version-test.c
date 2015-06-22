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

int main(int argc, char ** argv)
{
    int r;
    int i;

    for (i = 1; i < argc; i++) {
        r = rtv_set_replaytv_version(argv[i]);
        printf("%-20s  ", argv[i]);
        if (r < 0)
            printf("Failed\n");
        else
            printf("%d.%d.%d (%d)\n",
                   replaytv_version.major,
                   replaytv_version.minor,
                   replaytv_version.patch,
                   replaytv_version.build);
    }
    return 0;
}
