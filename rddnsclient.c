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
#include "httpclient.h"
#include "crypt.h"
#include <stdio.h>
#include <string.h>

struct decrypt_callback_data 
{
    char * plaintext_buffer;
    u32 plaintext_buffer_len;
    u32 plaintext_len;
    u32 t;
};

/* XXX this only handles one chunk */
static void decrypt_callback(unsigned char * cyphertext, size_t cyphertext_len, void * vp)
{
    struct decrypt_callback_data * d = vp;

    rtv_decrypt(cyphertext, cyphertext_len,
                d->plaintext_buffer, d->plaintext_buffer_len,
                &d->t, &d->plaintext_len, 0);
    free(cyphertext);
}

static int rddns_query(unsigned const char * path,
                       unsigned const char * querystring,
                       unsigned char * result_buffer, size_t result_buffer_len)
{
    struct decrypt_callback_data decrypt_data;
    unsigned char urlbuf[1024];
    unsigned char cyphertext[64];
    unsigned char * p;
    unsigned int i;
    struct hc * hc;
    u32 cyphertext_len;

    decrypt_data.plaintext_buffer     = result_buffer;
    decrypt_data.plaintext_buffer_len = result_buffer_len;

    if (rtv_encrypt(querystring, strlen(querystring),
                    cyphertext, sizeof cyphertext, &cyphertext_len, 0) < 0) {
        fprintf(stderr, "ERROR: encrypt failure\n");
        return -1;
    }

    /* rddns-rns.replaytv.net */
    if (strlen("http://64.124.73.112%s?q=") + 2*cyphertext_len + 1 > sizeof urlbuf) {
        fprintf(stderr, "ERROR rddns_lookup: urlbuf too small\n");
        return -1;
    }
    
    p = urlbuf + sprintf(urlbuf, "http://64.124.73.112%s?q=", path);
    for (i = 0; i < cyphertext_len; i++) {
        p += sprintf(p, "%02x", cyphertext[i]);
    }
    hc = hc_start_request(urlbuf);
    if (!hc) {
        fprintf(stderr, "ERROR rddns_lookup: hc_start_request %s failed\n",
                urlbuf);
        return -1;
    }
    if (hc_send_request(hc) < 0) {
        fprintf(stderr, "ERROR rddns_lookup: hc_send_request failed\n");
        return -1;
    }
    if (hc_get_status(hc)/100 != 2) {
        fprintf(stderr, "ERROR rddns_lookup http status %d\n",
                hc_get_status(hc));
        return -1;
    }
    if (hc_read_pieces(hc, decrypt_callback, &decrypt_data) < 0) {
        fprintf(stderr, "ERROR rddns_lookup read failed\n");
        return -1;
    }
    hc_free(hc);
    return 0;
}

int rddns_lookup(unsigned const char * ISN,
                 unsigned char * result_buffer, size_t result_buffer_len)
{
    unsigned char querystring[32];
    
    if (!ISN) {
        fprintf(stderr, "ERROR: rddns_lookup: null ISN\n");
        return -1;
    }
    if (strlen(ISN) != 17) {
        fprintf(stderr, "ERROR: rddns_lookup: invalid ISN\n");
        return -1;
    }
    sprintf(querystring, "isn=%s", ISN);

    if (rddns_query("/rd/servlet/gu", querystring,
                    result_buffer, result_buffer_len) < 0) {
        fprintf(stderr, "ERROR: rddns_lookup: rddns_query failed\n");
        return -1;
    }
    
    return 0;
}

int rddns_update(unsigned const char * ISN, unsigned short port,
                 unsigned char * result_buffer, size_t result_buffer_len)
{
    unsigned char querystring[64];
    
    if (!ISN) {
        fprintf(stderr, "ERROR: rddns_lookup: null ISN\n");
        return -1;
    }
    if (strlen(ISN) != 17) {
        fprintf(stderr, "ERROR: rddns_lookup: invalid ISN\n");
        return -1;
    }
    sprintf(querystring, "isn=%s&port=%d", ISN, port);

    if (rddns_query("/rd/servlet/ul", querystring,
                    result_buffer, result_buffer_len) < 0) {
        fprintf(stderr, "ERROR: rddns_lookup: rddns_query failed\n");
        return -1;
    }
    
    return 0;
}

