/*
 * Copyright (c) 1997 Kungliga Tekniska H�gskolan
 * (Royal Institute of Technology, Stockholm, Sweden). 
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 *
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *
 * 3. All advertising materials mentioning features or use of this software 
 *    must display the following acknowledgement: 
 *      This product includes software developed by Kungliga Tekniska 
 *      H�gskolan and its contributors. 
 *
 * 4. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 */

#include "krb5_locl.h"

RCSID("$Id$");

krb5_error_code
krb5_mk_error(krb5_context context,
	      krb5_error_code error_code,
	      const char *e_text,
	      const krb5_data *e_data,
	      const krb5_principal client,
	      const krb5_principal server,
	      time_t ctime,
	      krb5_data *reply)
{
    KRB_ERROR msg;
    unsigned char buf[1024];
    int32_t sec, usec;

    krb5_us_timeofday (context, &sec, &usec);

    memset(&msg, 0, sizeof(msg));
    msg.pvno     = 5;
    msg.msg_type = krb_error;
    msg.stime    = sec;
    msg.susec    = usec;
    if(ctime) {
	msg.ctime = &ctime;
    }
    msg.error_code = error_code - KRB5KDC_ERR_NONE;
    if (e_text)
	msg.e_text = (general_string*)&e_text;
    if (e_data)
	msg.e_data = (octet_string*)e_data;
    msg.realm = server->realm;
    msg.sname = server->name;
    if(client){
	msg.crealm = &client->realm;
	msg.cname = &client->name;
    }
    encode_KRB_ERROR(buf + sizeof(buf) - 1, sizeof(buf), &msg, &reply->length);
    reply->data = malloc(reply->length);
    memcpy(reply->data, buf + sizeof(buf) - reply->length, reply->length);
    return 0;
}
