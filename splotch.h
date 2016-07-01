/*****************************************************************************
Splotch

Copyright (C) 1992-2000  Duane K. Fields (duane@deepmagic.com)
changes Copyright (C) 2001 Mark Rages (markrages@yahoo.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*****************************************************************************/



#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define SPLOTCH_HISTORY   4    /* number of slots in old key queue */
#define SPLOTCH_TEMPLSIZ  2000         /* maximum number of templates */
#define SPLOTCH_DICTFILE  "main.dict"  /* name of dictionary file */
#define SPLOTCH_SYNFILE   "syn.dict"  /* name of synonym file */
#define SPLOTCH_PATH      ""
#define SPLOTCH_TEMPFILE  "/tmp/splotch"  /* name of temporary file */
#define DEBUG 0                /* debug flag */
#define VERBOSE 0              /* verbose errors */
#define BLANK 040

typedef struct splotch_st {
  struct splotch_template {
    long    toffset;           /* start of responses in file */
    char    tplate[400];       /* the template itself */
    int     priority;          /* how important key is 1=worst, 9=most */
    int     talts;             /* number of alternate replies (>0) */
    int     tnext;             /* next reply (1 <= tnext <= talts) */
  } templ[SPLOTCH_TEMPLSIZ];

  char   response[400];        /* response to be returned */
  char   my_nick[20];
  char   words[400];           /* a template has been matched, this is % */
  FILE   *dfile;               /* file pointer to main dictionary file */
  int    maxtempl;             /* templ[maxtempl] is last entry */
  int    oldkeywd[SPLOTCH_HISTORY];    /* queue of indices of most recent keys */

} splotch_t;

/* Function declarations */
int splotch_ask(splotch_t *me, char *person, char *question);
splotch_t *splotch_init(void);
void _sp_buildtempl(splotch_t *me);
int  _sp_usetempl(splotch_t *me, int i);
int  _sp_grline(char* infile, char* s1);
int  _sp_expand (char *s);
void _sp_swap(char *s, char *old, char *new);
void _sp_strlower(char *s);
char *lower(char *s, char *tmp);
int  _sp_fixfile(char *fname);
char *_sp_strcasestr (char *s1, char *s2);
int  _sp_trytempl(splotch_t *me, char *question);
char *_sp_phrasefind(char *string, char *searchstring);
void _sp_fix(splotch_t *me);
void _sp_gswap(splotch_t *me, char *old, char *new);
void _sp_shift(splotch_t *me, int base, int delta);
