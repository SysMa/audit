/* $Id: host.h,v 1.3 2001/05/12 18:44:04 mhe Exp $
 *
 * host.h -- DNS lookups of hostnames
 *
 * Yet Another FTP Client
 * Copyright (C) 1998-2001, Martin Hedenfalk <mhe@stacken.kth.se>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version. See COPYING for more details.
 */

#ifndef _host_h_included
#define _host_h_included

#include "syshdr.h"
#include "url.h"

typedef struct Host
{
	char *ipnum; /* aaa.bbb.ccc.ddd */
	char *hostname;
	char *ohostname;
	int port; /* ftp port number in network byte order */

	/* alternative address if we got an IPnumber and we couldn't resolv it */
	struct in_addr alt_h_addr;
	int alt_h_length;

	struct hostent *hep;
} Host;

Host *host_create(const url_t *urlp);
void host_destroy(Host *hostp);

int host_lookup(Host *hostp);
unsigned short int host_getport(const Host *hostp); /* returns port in network byte order */
unsigned short int host_gethport(const Host *hostp); /* returns port in host byte order */
const char *host_getip(const Host *hostp); /* returns IP number */
const char *host_getname(const Host *hostp); /* returns name as passed to host_set() */
const char *host_getoname(const Host *hostp); /* returns official name (as returned from gethostbyname()) */

#endif
