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

#ifndef RDDNSCLIENT_H
#define RDDNSCLIENT_H

#include <stdlib.h>

int rddns_lookup(unsigned const char * ISN,
                 unsigned char * resultbuf, size_t buflen);
int rddns_update(unsigned const char * ISN, unsigned short port,
                 unsigned char * result_buffer, size_t result_buffer_len);

#endif
