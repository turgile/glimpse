/*
 *  template.h - SOIF template processing support
 *
 *  Darren Hardy, hardy@cs.colorado.edu, July 1994
 *
 *  $Id: template.h,v 1.2 2006/02/03 16:59:14 golda Exp $
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
#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

#include <stdio.h>
#include "config.h"

/* Attribute-Value pair */
struct attr_value {
	char *attribute;	/* Attribute string; '\0' terminated */
	char *value;		/* Value data; not '\0' terminated */
	size_t vsize;		/* # of bytes in the value data */
	size_t offset;		/* Starting byte of the value data in input */
};
typedef struct attr_value AVPair;

/* List of Attribute-Value pairs */
struct attr_value_list {
	AVPair *data;
	struct attr_value_list *next;
};
typedef struct attr_value_list AVList;

/* SOIF Template structure */
struct template {
	char *template_type;	/* type of template */
	char *url;		/* URL for template */
	AVList *list;		/* List of Attribute-Value pairs */
	size_t offset;		/* Starting byte of the template; the @ */
	size_t length;		/* # of bytes that the template covers */
};
typedef struct template Template;


/* Common Attribute Tags */
#define T_ABSTRACT	"Abstract"
#define T_AUTHOR	"Author"
#define T_FILESIZE	"File-Size"
#define T_FULLTEXT	"Full-Text"
#define T_GENERATION	"Generation-Time"
#define T_GHOST		"Gatherer-Host"
#define T_GNAME		"Gatherer-Name"
#define T_GVERSION	"Gatherer-Version"
#define T_KEYS		"Keywords"
#define T_LMT		"Last-Modification-Time"
#define T_MD5		"MD5"
#define T_NESTED	"Nested-Filename"
#define T_PARTTEXT	"Partial-Text"
#define T_REFERENCE	"References"
#define T_REFRESH	"Refresh-Rate"
#define T_RELATED	"Related"
#define T_TITLE		"Title"
#define T_TTL		"Time-to-Live"
#define T_TYPE		"Type"
#define T_UPDATE	"Update-Time"
#define T_URL		"URL"

/* Backwards compatibility */
#define T_FILETYPE	T_TYPE
#define T_TIMESTAMP	T_LMT

/* Common time_t numbers */
#define HOUR	((time_t) 60 * 60)
#define DAY	((time_t) HOUR * 24)
#define WEEK    ((time_t) DAY * 7)
#define MONTH	((time_t) WEEK * 4)
#define YEAR	((time_t) MONTH * 12)

#ifndef _PARAMS
#define _PARAMS(ARGS) ARGS
#endif /* _PARAMS */   

/* Templates */
Template *create_template _PARAMS((char *, char *));
Template *embed_template _PARAMS((Template *, Template *));
void free_template _PARAMS((Template *));

/* Attribute-Value pairs */
AVList *create_AVList _PARAMS((char *, char *, int));
void add_AVList _PARAMS((AVList *, char *, char *, int));
void FAST_add_AVList _PARAMS((AVList *, char *, char *, int));
void append_AVList _PARAMS((AVList *, char *, char *, int));
AVPair *extract_AVPair _PARAMS((AVList *, char *));
AVList *merge_AVList _PARAMS((AVList *, AVList *));
AVList *sort_AVList _PARAMS((AVList *));
AVList *sink_embedded _PARAMS((AVList *));
int exists_AVList _PARAMS((AVList *, char *));
void free_AVPair _PARAMS((AVPair *));
void free_AVList _PARAMS((AVList *));

/* Printing Templates */
Buffer *init_print_template _PARAMS((FILE *));
void print_template _PARAMS((Template *));
void print_template_header _PARAMS((Template *));
void print_template_body _PARAMS((Template *));
void print_template_trailer _PARAMS((Template *));
void finish_print_template _PARAMS(());

/* Parsing Templates */
void init_parse_template_file _PARAMS((FILE *));
void init_parse_template_string _PARAMS((char *, int));
Template *parse_template _PARAMS(());
void finish_parse_template _PARAMS(());
int is_parse_end_of_input _PARAMS(());

#endif /* _TEMPLATE_H_ */
