static char rcsid[] = "$Id: iafa2soif.c,v 1.1 1999/11/03 21:41:04 golda Exp $";
/*
 *  iafa2soif - Converts IAFA templates to SOIF.
 *
 *  Usage: iafa2soif url local-file
 *
 *  Darren Hardy, hardy@cs.colorado.edu, June 1994
 *
 *  ----------------------------------------------------------------------
 *  Copyright (c) 1994, 1995.  All rights reserved.
 *  
 *          Mic Bowman of Transarc Corporation.
 *          Peter Danzig of the University of Southern California.
 *          Darren R. Hardy of the University of Colorado at Boulder.
 *          Udi Manber of the University of Arizona.
 *          Michael F. Schwartz of the University of Colorado at Boulder. 
 *  
 *  This copyright notice applies to all code in Harvest other than
 *  subsystems developed elsewhere, which contain other copyright notices
 *  in their source text.
 *  
 *  The Harvest software was developed by the Internet Research Task
 *  Force Research Group on Resource Discovery (IRTF-RD).  The Harvest
 *  software may be used for academic, research, government, and internal
 *  business purposes without charge.  If you wish to sell or distribute
 *  the Harvest software to commercial clients or partners, you must
 *  license the software.  See
 *  http://harvest.cs.colorado.edu/harvest/copyright,licensing.html#licensing.
 *  
 *  The Harvest software is provided ``as is'', without express or
 *  implied warranty, and with no support nor obligation to assist in its
 *  use, correction, modification or enhancement.  We assume no liability
 *  with respect to the infringement of copyrights, trade secrets, or any
 *  patents, and are not responsible for consequential damages.  Proper
 *  use of the Harvest software is entirely the responsibility of the user.
 *  
 *  For those who are using Harvest for non-commercial purposes, you may
 *  make derivative works, subject to the following constraints:
 *  
 *  - You must include the above copyright notice and these accompanying 
 *    paragraphs in all forms of derivative works, and any documentation 
 *    and other materials related to such distribution and use acknowledge 
 *    that the software was developed at the above institutions.
 *  
 *  - You must notify IRTF-RD regarding your distribution of the 
 *    derivative work.
 *  
 *  - You must clearly notify users that your are distributing a modified 
 *    version and not the original Harvest software.
 *  
 *  - Any derivative product is also subject to the restrictions of the 
 *    copyright, including distribution and use limitations.
 */
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "url.h"
#include "template.h"

static void usage()
{
	fprintf(stderr, "Usage: iafa2soif url local-file\n");
	exit(1);
}

#define isiafa(c)	(isalnum(c) || ((c) == '-') || ((c) == '#'))

static void do_iafatosoif(url, filename)
char *url;
char *filename;
{
	char buf[BUFSIZ], *s, *p;
	char attr[BUFSIZ];
	char value[BUFSIZ];
	int i;
	Template *t;
	FILE *fp;
	URL *up;

	if ((up = url_open(url)) == NULL) {
		errorlog("Cannot open URL: %s\n", url);
		return;
	}

	/* Build the template */
	t = create_template(NULL, up->url);

	/* Read the file and build a SOIF template from it */
	if ((fp = fopen(filename, "r")) == NULL) {
		log_errno(filename);
		url_close(up);
		return;
	}
	attr[0] = '\0';
	while (fgets(buf, BUFSIZ, fp)) {
		if ((s = strrchr(buf, '\n')) != NULL)
			*s = '\0';
		if (strlen(buf) < 1)
			continue;
		if ((s = strchr(buf, ':')) == NULL) {
			append_AVList(t->list, attr, buf, strlen(buf));
			continue;
		}
		for (p = buf, i = 0; p < s && isiafa(*p); p++, i++)
			attr[i] = *p;
		attr[i] = '\0';
		if (i < 1)
			continue;	/* null attribute */
		while (*s != '\0' && (*s == ':' || isspace(*s)))
			s++;
		if (strlen(s) < 1)	/* empty line */
			continue;
		strcpy(value, s);
		if (t->list)
			append_AVList(t->list, attr, value, strlen(value));
		else
			t->list = create_AVList(attr, value, strlen(value));
		
	}
	fclose(fp);

	/* Print out the template */
	(void) init_print_template(stdout);
	print_template(t);
	finish_print_template();
	free_template(t);
	url_close(up);
	return;
}

int main(argc, argv)
int argc;
char *argv[];
{
	char *url, *filename;

	if (argc != 3)
		usage();
	url = strdup(argv[1]);
	filename = strdup(argv[2]);

	init_log(stderr, stderr);
	init_url();
	do_iafatosoif(url, filename);
	finish_url();
	exit(0);
}
