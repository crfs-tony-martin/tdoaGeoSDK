/*-----------------------------------------------------------------------------
 * (c) CRFS Limited, 2011. All rights reserved.
 *
 * This software is the property of CRFS Limited and may not be copied or
 * reproduced otherwise than on to a single hard disk for backup or
 * archival purposes. The source code is confidential information and must
 * not be disclosed to third parties or used without the express written
 * permission of CRFS Limited.
 *
 * Filename : json.c
 * Author : Dan Timson
 *
 * Purpose: JSON strings.
 *
 * Description:
 *---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <errno.h>

/* TODO: reduce included hdr's if poss */
#ifdef _WIN32
	#ifndef __BORLANDC__
//		#include <sys/time.h>
	#endif
    #include <ws2spi.h>
    #include <windows.h>
    #include <winbase.h>
    #include <winsock2.h>
#else
    #include <sys/time.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include "packets.h"
#include "crfs_debug.h"

typedef struct {
    char *buf;
    int32_t bufsiz;
    char *ptr;
    int indent;
    int format;
    int midline;
} Json;

/* local prototypes */
static Json *json_create(void);
static int32_t json_adjust(Json *json, int32_t max_inc);
static int32_t json_append(Json *json, const char* str);
static int32_t json_appendf(Json *json, const char* fmt, ...);
static void json_append_vector(Json *json, const char *name, int32_t atype,
                               void *adata, int32_t len);
static char* _STDCALL json_create_from_packet_impl(T_PACKET *packet, int32_t format);

#define INDENT 2
#define CHUNK_SIZE 256
#define SBUF_LEN 1024
#define NTRUNC 4
#define MIN_TRUNC 15

#define JSON_FORMAT_FULL 1
#define JSON_FORMAT_TRUNCATE 2


/**
    Convert packet to json string, with truncated data.
*/
LIBSPEC char* _STDCALL
json_create_from_packet(T_PACKET *packet)
{
    return json_create_from_packet_impl(packet, JSON_FORMAT_TRUNCATE);
}

/**
    Convert packet to json string, with full data.
*/
LIBSPEC char* _STDCALL
json_full_create_from_packet(T_PACKET *packet)
{
    return json_create_from_packet_impl(packet, JSON_FORMAT_FULL);
}

/**
    Convert packet to json string.
*/
static char* _STDCALL
json_create_from_packet_impl(T_PACKET *packet, int32_t format)
{
    Json *json;
    char pstr[32];
    int32_t len, packet_type, first, fname, fid, sec, aname;
    T_PARAM_DATA_TYPE atype;
    char *retval;
    void *adata;

    json = json_create();
    json->format = format;
    packet_type = packet_read(packet);
    if (packet_type == 0) {
        json_append(json, "[INCOMPLETE PACKET]\n");
        retval = json->buf;
        Free(json);
        return retval;
    }

    json_append(json, "{\n");
    json_appendf(json, "\"PACKET_TYPE\":\"%s\",\n", packet_key_to_str(packet_type, pstr));
    json_appendf(json, "\"PACKET_ID\":%d,\n", packet_get_packet_id(packet));
    json_appendf(json, "\"PACKET_FORMAT\":%d,\n", packet_get_packet_format(packet));
    json_appendf(json, "\"PACKET_SIZE\":%d,\n", packet_get_packet_size(packet));
    sec = packet_get_packet_sec(packet);
    json_appendf(json, "\"PACKET_TIME_SEC\":%d,\n", sec);
    json_appendf(json, "\"PACKET_TIME_NANO_SEC\":%d,\n", packet_get_packet_nanosec(packet));
#ifndef _WIN32
    json_appendf(json, "\"PACKET_TIME_STR\":%s", ctime_r((time_t*) &sec, pstr));
#endif

    while (packet_get_next_field(packet, &fname, &fid))
    {
        json_append(json, "\"FIELD\":\n");
        json_append(json, "{\n");
        json_appendf(json, "\"FIELD_NAME\":\"%s\",\n", packet_key_to_str(fname, pstr));
        json_appendf(json, "\"FIELD_ID\":\"%d\",\n", fid);

        first = 1;
        while (packet_get_next_param(packet, &aname, &atype, &adata, &len))
        {
            if (first)
                first = 0;
            else
                json_append(json, ",");

            if (PARAM_IS_SCALAR(atype)) {
                json_appendf(json, "\"%s\":", packet_key_to_str(aname, pstr));

                switch (atype) {
                case PARAM_INT: json_appendf(json, "%d", *(int32_t*)(adata)); break;
                case PARAM_UNSIGNED_INT: json_appendf(json, "%u", *(uint32_t*)(adata)); break;
                case PARAM_STRING: json_appendf(json, "\"%s\"", (char*)(adata)); break;
                default: json_append(json, "\"??\""); break;
                }
            } else {
                packet_key_to_str(aname, pstr);
                json_append_vector(json, pstr, atype, adata, len);
            }

			//json_append(json, "\n");
        }
        if (!first)
            json_append(json, "\n");

        json_append(json, "}\n");
    }

    json_append(json, "}");
    retval = json->buf;
    Free(json);
    return retval;
}


/**
    Free a json string.
*/
LIBSPEC void _STDCALL json_free(char *json)
{
    Free(json);
}


/**
    Print32_t human readable version of packet to stdout.
*/
LIBSPEC void _STDCALL
packet_print(T_PACKET *packet)
{
    char *json;
    printf("<<<<PACKET BEGIN  >>>>\r\n");
    json = json_create_from_packet(packet);
	printf("%s",json);
    json_free(json);
    printf("<<<<PACKET END  >>>>\r\n");
}

/* local functions */

static Json *
json_create()
{
    Json *self;

    if ( (self = (Json*)Malloc(sizeof(Json))) == NULL)
        return NULL;

    /* allocate buffer */
    self->bufsiz = CHUNK_SIZE;
    if ( (self->buf = (char*)Malloc(self->bufsiz)) == NULL) {
        Free(self);
        return NULL;
    }

    self->ptr = self->buf;
    *self->ptr = 0; /* start with empty string */
    self->indent = 0;
    self->format = JSON_FORMAT_FULL;
    self->midline = 0;

    return self;}

/**
   \param[in] max_inc maximum increase in buffer length
 */
static int32_t
json_adjust(Json *json, int32_t max_inc)
{
    int32_t cur_siz; /* current used size of buf including final null */
    int32_t new_siz; /* new used size of buf including final null */
    char *new_buf;

    cur_siz = json->ptr - json->buf + 1;
    new_siz = cur_siz + max_inc;
    if (new_siz > json->bufsiz) {
         new_siz += CHUNK_SIZE; /* headroom */

        new_buf = (char*)realloc(json->buf, new_siz);
        if (new_buf == NULL) {
            print_error("json_append: realloc failed (%d bytes requested)", new_siz);
            return -1;
        } else {
            json->buf = new_buf;
            json->ptr = json->buf + cur_siz - 1;
            json->bufsiz = new_siz;
        }
    }

    return ERROR_NONE;
}

static int32_t
json_append(Json *json, const char* str)
{
    int32_t indent = json->indent;
    char *position;
    int32_t len = strlen(str);
    int32_t i, error, max_inc;

    /* ensure buffer is large enough */
    max_inc = indent + len + 2;
    if ( (error = json_adjust(json, max_inc)) != ERROR_NONE)
        return error;

    position = json->ptr;

    /* adjust indent */
    if ((str[0] == '}') && (indent >= 0))
        indent -= INDENT;
    if (indent < 0)
        indent = 0;

    if (!json->midline) {
        for (i=0; i<indent; i++)
            *position++ = ' ';
    }

    if (str[len-1] == '\n') {
        json->midline = 0;
        len--;
    } else
        json->midline = 1;

	/* do copy and add line end and final null */
    memcpy(position, str, len);
    position += len;
    if (!json->midline) {
        *position++ = '\r';
        *position++ = '\n';
    }
    *position = 0;

    if (str[0] == '{')
        indent += INDENT;

    json->indent = indent;
    json->ptr = position;

    return ERROR_NONE;
}

static int32_t
json_appendf(Json *json, const char* fmt, ...)
{
    va_list     ap;
    int n; /* no of bytes that _would_ be written, excluding final null */
    int32_t retval;
    char sbuf[SBUF_LEN];

    va_start(ap, fmt);
    if ( (n = vsnprintf(sbuf, SBUF_LEN, fmt, ap)) > SBUF_LEN - 1) {
        /* try again with bigger buffer */
        char *bigbuf;

        if ( (bigbuf = (char*)Malloc(n+1)) == NULL)
            return -1;
        vsnprintf(bigbuf, n+1, fmt, ap);
        print_warning("json_appendf buffer increase: %d > %d", n+1, SBUF_LEN);
        retval = json_append(json, bigbuf);
        Free(bigbuf);
    } else {
        retval = json_append(json, sbuf);
    }
    va_end(ap);

    return retval;
}

static void
json_append_vector(Json *json, const char *name, int32_t atype, void *adata, int32_t len)
{
    int i;
    const int trunc = (json->format == JSON_FORMAT_TRUNCATE) && (len >= MIN_TRUNC);
#if 0
    /* adjust len */
    switch (atype) {
    case PARAM_DATA_RAW:
    case PARAM_DATA_UNSIGNED_8:
    case PARAM_DATA_SIGNED_8:
        len *= 4; /* no of bytes */
        break;
    case PARAM_DATA_UNSIGNED_16:
    case PARAM_DATA_SIGNED_16:
        len *= 2; /* no of int16's */
        break;
    }
#endif
    /* print each element*/
    json_appendf(json, "\"%s[%d]\":[", name, len);

    for (i=0; i<len; i++)
    {
        if (trunc && (i == NTRUNC)) {
            json_append(json, "...,");
        } else if (trunc && (i > NTRUNC) && (i < len-NTRUNC)) {
            ; /* do nothing */
        } else {
            char c = (i < len-1) ? ',' : ']';
            uint32_t u;
            int32_t d;
            switch (atype) {
            case PARAM_DATA_RAW:
            case PARAM_DATA_UNSIGNED_8:
                u = ((unsigned char*)adata)[i];
                json_appendf(json, "%u%c", u, c);
                break;
            case PARAM_DATA_SIGNED_8:
                d = ((char*)adata)[i];
                d = (d < 128) ? d : d-256;
                json_appendf(json, "%d%c", d, c);
                break;
            case PARAM_DATA_UNSIGNED_16:
                u = ((uint16_t*)adata)[i];
                json_appendf(json, "%u%c", u, c);
                break;
            case PARAM_DATA_SIGNED_16:
                d = ((int16_t*)adata)[i];
                json_appendf(json, "%d%c", d, c);
                break;
            case PARAM_DATA_UNSIGNED_32:
                u = ((uint32_t*)adata)[i];
                json_appendf(json, "%u%c", u, c);
                break;
            case PARAM_DATA_SIGNED_32:
                d = ((int32_t*)adata)[i];
                json_appendf(json, "%d%c", d, c);
                break;
            default:
                u = ((uint32_t*)adata)[i];
                json_appendf(json, "0x%x%c", u, c);
                break;
            }
        }
    }
}
