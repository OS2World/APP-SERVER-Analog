/*** analog 2.11 ***/
/* Please read Readme.html, or http://www.statslab.cam.ac.uk/~sret1/analog/  */

/*** output2.c; subsiduary output functions. ***/
/* See also output.c */

#include "analhea2.h"

/* used in lots of functions, so I declare it here */
extern char *lngstr[NOLNGSTRS];

/*** The first function prints the "goto" line; links to all reports except
     possibly one (the one we're on). If called gotos('\0') won't omit one.
     If called gotos('z') will omit 'Top'. ***/

void gotos(FILE *outf, char c)
{
  extern char reportorder[];
  extern flag bq, Bq, cq, dq, Dq, eq, fq, hq, Hq, iq, tq, mq, oq, rq, Sq, Wq;
  extern flag xq;

  char *i;

  if (xq) {  /* NB Have already tested aq == HTML */
    fprintf(outf, "\n\n<p>(<b>%s</b>", lngstr[goto_]);

    if (c != 'z')
      fprintf(outf, ": <a HREF=\"#Top\">%s</a>", lngstr[top_]);

    for (i = reportorder; *i != '\0'; i++) {
      if (c != *i) {   /* o/wise we don't want this one */
	switch(*i) {
	case 'b':
	  if (bq)
	    fprintf(outf, "%s <a HREF=\"#Browser\">%s</a>", lngstr[colon_],
		    lngstr[browsum_]);
	  break;
	case 'B':
	  if (Bq)
	    fprintf(outf, "%s <a HREF=\"#FullBrowser\">%s</a>",
		    lngstr[colon_], lngstr[browrep_]);
	  break;
	case 'c':
	  if (cq)
	    fprintf(outf, "%s <a HREF=\"#Status\">%s</a>", lngstr[colon_],
		    lngstr[statrep_]);
	  break;
	case 'd':
	  if (dq)
	    fprintf(outf, "%s <a HREF=\"#Daily\">%s</a>", lngstr[colon_],
		    lngstr[daysum_]);
	  break;
	case 'D':
	  if (Dq)
	    fprintf(outf, "%s <a HREF=\"#FullDaily\">%s</a>", lngstr[colon_],
		    lngstr[dayrep_]);
	  break;
	case 'e':
	  if (eq)
	    fprintf(outf, "%s <a HREF=\"#Error\">%s</a>", lngstr[colon_],
		    lngstr[errrep_]);
	  break;
	case 'f':
	  if (fq)
	    fprintf(outf, "%s <a HREF=\"#Referrer\">%s</a>", lngstr[colon_],
		    lngstr[refrep_]);
	  break;
	case 'H':
	  if (Hq)
	    fprintf(outf, "%s <a HREF=\"#FullHourly\">%s</a>",
		    lngstr[colon_], lngstr[hourrep_]);
	  break;
	case 'h':
	  if (hq)
	    fprintf(outf, "%s <a HREF=\"#Hourly\">%s</a>", lngstr[colon_],
		    lngstr[hoursum_]);
	  break;
	case 'i':
	  if (iq)
	    fprintf(outf, "%s <a HREF=\"#Directory\">%s</a>", lngstr[colon_],
		    lngstr[dirrep_]);
	  break;
	case 'm':
	  if (mq)
	    fprintf(outf, "%s <a HREF=\"#Monthly\">%s</a>", lngstr[colon_],
		    lngstr[monthrep_]);
	  break;
	case 'o':
	  if (oq)
	    fprintf(outf, "%s <a HREF=\"#Domain\">%s</a>", lngstr[colon_],
		    lngstr[domrep_]);
	  break;
	case 'r':
	  if (rq)
	    fprintf(outf, "%s <a HREF=\"#Request\">%s</a>", lngstr[colon_],
		    lngstr[reqrep_]);
	  break;
	case 'S':
	  if (Sq)
	    fprintf(outf, "%s <a HREF=\"#Host\">%s</a>", lngstr[colon_],
		    lngstr[hostrep_]);
	  break;
	case 't':
	  if (tq)
	    fprintf(outf, "%s <a HREF=\"#FileType\">%s</a>", lngstr[colon_],
		    lngstr[typerep_]);
	  break;
	case 'W':
	  if (Wq)
	    fprintf(outf, "%s <a HREF=\"#Weekly\">%s</a>", lngstr[colon_],
		    lngstr[weekrep_]);
	  break;
	}   /* end switch */
      }     /* end if this i wanted */
    }       /* end for i */

    fprintf(outf, ")\n");

  }         /* end if xq */
}           /* end function gotos() */

/*** Next, to print strings with HTML reserved characters translated ***/

void htmlputc(char c, FILE *outf)
{
  if (c == '<')
    fprintf(outf, "&lt;");
  else if (c == '>')
    fprintf(outf, "&gt;");
  else if (c == '&')
    fprintf(outf, "&amp;");
  else if (c == '"')
    fprintf(outf, "&quot;");
  else
    putc(c, outf);
}

void htmlfprintf(FILE *outf, char string[MAXSTRINGLENGTH])
{
  extern flag html2;

  char *c;

  for (c = string; *c != '\0'; c++) {
    if (*c == '\\' && *(c + 1) != '\0') {
      html2 = OFF;
      putc(*(++c), outf);
    }
    else
      htmlputc(*c, outf);
  }
}

/*** Print some character to the same length as a (poss. HTML) string ***/
/*** Assume string contains no &'s except as markup ***/

size_t htmlstrlen(char *s)
{
  size_t i = 0;
  flag f = TRUE;

  for ( ; *s != '\0'; s++) {
    if (*s == '&')
      f = OFF;
    else if (*s == ';')
      f = ON;
    if (f)
      i++;
  }
  return(i);
}

void matchlength(FILE *outf, char *s, char c)
{
  int i;

  for (i = (int)htmlstrlen(s); i > 0; i--)
    fprintf(outf, "%c", c);
}

/*** Date printing routine ***/

size_t datefmtlen(char *fmt)
{
  extern char dayname[7][11];
  extern char monthname[12][12];

  size_t i = 0;
  char *c;

  for (c = fmt; *c != '\0'; c++) {
    if (*c == '%' && *(c + 1) != '\0') {
      c++;
      if (*c == 'd' || *c == 'D' || *c == 'y' || *c == 'h' || *c == 'H' ||
	  *c == 'n')
	i += 2;
      else if (*c == 'Y')
	i += 4;
      else if (*c == 'm')
	i += htmlstrlen(monthname[0]);
      else if (*c == 'w')
	i += htmlstrlen(dayname[0]);
    }
    else
      i += 1;
  }
  return(i);
}

void dateprintf(FILE *outf, char *fmt, int date, int month, int year, int hr,
		int min)
{
  extern char dayname[7][11];
  extern char monthname[12][12];

  char *c;

  for (c = fmt; *c != '\0'; c++) {
    if (*c == '%' && *(c + 1) != '\0') {
      c++;
      switch (*c) {
      case 'd':
	fprintf(outf, "%2d", date);
	break;
      case 'D':
	fprintf(outf, "%02d", date);
	break;
      case 'm':
	fprintf(outf, "%s", monthname[month]);
	break;
      case 'y':
	fprintf(outf, "%02d", year % 100);
	break;
      case 'Y':
	fprintf(outf, "%d", year);
	break;
      case 'h':
	fprintf(outf, "%2d", hr);
	break;
      case 'H':
	fprintf(outf, "%02d", hr);
	break;
      case 'n':
	fprintf(outf, "%02d", min);
	break;
      case 'w':
	fprintf(outf, "%s", dayname[dayofdate(date, month, year)]);
	break;
      }
    }
    else
      fprintf(outf, "%c", *c);
  }
}

/*** Now a little routine to find the correct divider for large numbers of
     bytes. Also sets bprefix[0] as a side effect. ***/

double finddivider(double bytes, char *bprefix)
{
  extern flag rawbytes;

  double bdivider;

  if (rawbytes)
    bdivider = 1.0;
  else
    for (bdivider = 1; bytes / bdivider >= 999999.5;
	 bdivider *= 1024)
      ;  /* run bdivider to right multiplier */

  if (bdivider == 1.0)
    *bprefix = '\0';
  else if (bdivider == 1024.0)
    *bprefix = 'k';
  else if (bdivider == 1048576.0)
    *bprefix = 'M';
  else if (bdivider == 1073741824.0)
    *bprefix = 'G';
  else if (bdivider == 1099511627776.0)
    *bprefix = 'T';
  else       /* 10^6 terabytes should be enough. Just about. */
    *bprefix = '?';

  return(bdivider);
}

/*** print a line across the page, assuming ASCII mode ***/

void asciiline(FILE *outf)
{
  extern int pagewidth;

  int i;

  for (i = 0; i < pagewidth; i++)
    fprintf(outf, "-");
  fprintf(outf, "\n\n");
}

/*** a barchart bar, length n, within <pre><tt> ***/

void barplot(FILE *outf, int n)
{
  extern int aq;
  extern flag graphical;
  extern char *imagedir;
  extern char markchar;

  int i, k;
  flag first = TRUE;

  if (aq || !graphical) {
    for ( ; n > 0; n--)
      fprintf(outf, "%c", markchar);
  }

  else {
    for (k = 32; k >= 1; k /= 2) {
      while (n >= k) {
	fprintf(outf, "<img src=\"");
	htmlfprintf(outf, imagedir);
	fprintf(outf, "bar%d.gif\" alt=\"", k);
	if (first) {
	  for (i = n; i > 0; i--)
	    htmlputc(markchar, outf);
	  first = FALSE;
	}
	fprintf(outf, "\">");
	n -= k;
      }
    }
  }
}

/*** Left hand two columns in OUTPUT PREFORMATTED ***/
/* NB: Check (aq == PREFORMATTED) elsewhere */

void precols(FILE *outf, char *wantcols, char codeletter, flag byq, flag pageq)
{
  extern char *presep;

  char *cols;

  fprintf(outf, "%c%s", codeletter, presep);
  for (cols = wantcols; *cols != '\0'; cols++) {
    switch(*cols) {
    case 'R':
    case 'r': 
      fprintf(outf, "%c", *cols);
      break;
    case 'P':
    case 'p':
      if (pageq)
	fprintf(outf, "%c", *cols);
      break;
    case 'b':
    case 'B':
      if (byq)
	fprintf(outf, "%c", *cols);
      break;
    }
  }
  fprintf(outf, "%s", presep);
}

/*** Two functions to print R,r,P,p,B,b cols given various parameters ***/

void printcolheads(FILE *outf, char *wantcols, int fieldwidth, int pfieldwidth,
		   int bfieldwidth, char bprefix[2], char name[20], int width,
		   char type, flag byq, flag pageq, flag name1st)
{   /* assume aq != PREFORMATTED already tested */
  char *cols;
  int i;

  if (name1st) {
    for (i = (int)htmlstrlen(name); i < width; i++)
      fprintf(outf, " ");
    fprintf(outf, "%s: ", name);
  }
  for (cols = wantcols; *cols != '\0'; cols++) {
    switch(*cols) {
    case 'R':
      for (i = (int)strlen(lngstr[(type == 'c' || type == 'e')?noccs_:nreqs_]);
	   i < fieldwidth; i++)
	fprintf(outf, " ");
      if (type == 'o')
	fprintf(outf, " %s : ", lngstr[nreqs_]);
      else if (type == 'c' || type == 'e')
	fprintf(outf, "%s: ", lngstr[noccs_]);
      else
	fprintf(outf, "%s: ", lngstr[nreqs_]);
      break;
    case 'r':
      for (i = (int)strlen(lngstr[(type == 'c' || type == 'e')?poccs_:preqs_]);
	   i < 6; i++)
	fprintf(outf, " ");
      if (type == 'o')
	fprintf(outf, " %s : ", lngstr[preqs_]);
      else if (type == 'c' || type == 'e')
	fprintf(outf, "%s: ", lngstr[poccs_]);
      else
	fprintf(outf, "%s: ", lngstr[preqs_]);
      break;
    case 'P':
      if (pageq) {
	for (i = (int)strlen(lngstr[npgs_]); i < pfieldwidth; i++)
	  fprintf(outf, " ");
	if (type == 'o')
	  fprintf(outf, " %s : ", lngstr[npgs_]);
	else
	  fprintf(outf, "%s: ", lngstr[npgs_]);
      }
      break;
    case 'p':
      for (i = (int)strlen(lngstr[ppgs_]); i < 6; i++)
	fprintf(outf, " ");
      if (pageq) {
	if (type == 'o')
	  fprintf(outf, " %s : ", lngstr[ppgs_]);
	else
	  fprintf(outf, "%s: ", lngstr[ppgs_]);
      }
      break;
    case 'B':
      if (byq) {
	for (i = 1 + (int)strlen(lngstr[nbytes_]); i < bfieldwidth; i++)
	  fprintf(outf, " ");
	if (type == 'o')
	  fprintf(outf, " %s%s : ", (bprefix[0] == '\0')?" ":bprefix,
		  lngstr[nbytes_]);
	else
	  fprintf(outf, "%s%s: ", (bprefix[0] == '\0')?" ":bprefix,
		  lngstr[nbytes_]);
      }
      break;
    case 'b':
      if (byq) {
	for (i = (int)strlen(lngstr[pbytes_]); i < 6; i++)
	  fprintf(outf, " ");
	if (type == 'o')
	  fprintf(outf, " %s : ", lngstr[pbytes_]);
	else
	  fprintf(outf, "%s: ", lngstr[pbytes_]);
      }
      break;
    }
  }
  if (!name1st) {
    for (i = (int)htmlstrlen(name); i < width; i++)
      fprintf(outf, " ");
    fprintf(outf, "%s", name);
  }
  fprintf(outf, "\n");

  if (name1st) {
    for (i = 0; i < width; i++)
      fprintf(outf, "-");
    fprintf(outf, "  ");
  }

  for (cols = wantcols; *cols != '\0'; cols++) {
    switch(*cols) {
    case 'R':
      for (i = 1; i <= fieldwidth + 2 * (type == 'o'); i++)
	fprintf(outf, "-");
      fprintf(outf, "  ");
      break;
    case 'r':
      fprintf(outf, "%s------  ", (type == 'o')?"--":"");
      break;
    case 'P':
      if (pageq) {
	for (i = 1; i <= pfieldwidth + 2 * (type == 'o'); i++)
	  fprintf(outf, "-");
	fprintf(outf, "  ");
      }
      break;
    case 'p':
      if (pageq)
	fprintf(outf, "%s------  ", (type == 'o')?"--":"");
      break;
    case 'B':
      if (byq) {
	for (i = 1; i <= bfieldwidth + 2 * (type == 'o'); i++)
	  fprintf(outf, "-");
	fprintf(outf, "  ");
      }
      break;
    case 'b':
      if (byq)
	fprintf(outf, "%s------  ", (type == 'o')?"--":"");
      break;
    }
  }
  if (!name1st)
    matchlength(outf, name, '-');
  fprintf(outf, "\n");
}

void printcols(FILE *outf, char *wantcols, int reqs, int pages, double bytes,
	       int fieldwidth, int pfieldwidth, int bfieldwidth,
	       double bdivider, int totreqs, int totpages, double totbytes,
	       char type, flag byq, flag pageq)
{
  extern char *presep;
  extern char repsepchar, decpoint;
  extern flag aq;

  char *cols;
  double pc;
  int pc1, pc2;
  int k;

  for (cols = wantcols; *cols != '\0'; cols++) {
    switch(*cols) {
    case 'R':
      if (aq == PREFORMATTED)
	fprintf(outf, "%d%s", reqs, presep);
      else {
	if (type == 'o')
	  fprintf(outf, " ");
	else if (type == 'O')
	  fprintf(outf, "(");
	int3printf(outf, reqs, repsepchar, fieldwidth);
	if (type == 'o')
	  fprintf(outf, " : ");
	else if (type == 'O')
	  fprintf(outf, "): ");
	else
	  fprintf(outf, ": ");
      }
      break;
    case 'r':
      if (totreqs == 0)
	pc = 0;
      else
	pc = (reqs + 0.0) / ((totreqs + 0.0) / 10000);
      pc1 = ((int)(pc + 0.5)) / 100;     /* whole no. of %reqs */
      pc2 = ((int)(pc + 0.5)) % 100;     /* remaining 100ths. */
      if (aq != PREFORMATTED) {
	if (type == 'o')
	  fprintf(outf, " ");
	else if (type == 'O')
	  fprintf(outf, "(");
      }
      if (pc1 == 100) {
	if (aq == PREFORMATTED)
	  fprintf(outf, "100.00");
	else
	  fprintf(outf, "  100%%");
      }
      else if (pc1 > 0 || pc2 > 0) {
	if (aq == PREFORMATTED)
	  fprintf(outf, "%d.%02d", pc1, pc2);
	else
	  fprintf(outf, "%2d%c%02d%%", pc1, decpoint, pc2);
      }
      else if (aq == PREFORMATTED)
	fprintf(outf, "0.00");
      else
	fprintf(outf, "      ");
      if (aq == PREFORMATTED)
	fprintf(outf, "%s", presep);
      else if (type == 'o')
	fprintf(outf, " : ");
      else if (type == 'O')
	fprintf(outf, "): ");
      else
	fprintf(outf, ": ");
      break;
    case 'P':
      if (pageq) {
	if (aq == PREFORMATTED)
	  fprintf(outf, "%d%s", pages, presep);
	else {
	  if (type == 'o')
	    fprintf(outf, " ");
	  else if (type == 'O')
	    fprintf(outf, "(");
	  int3printf(outf, pages, repsepchar, pfieldwidth);
	  if (type == 'o')
	    fprintf(outf, " : ");
	  else if (type == 'O')
	    fprintf(outf, "): ");
	  else
	    fprintf(outf, ": ");
	}
      }
      break;
    case 'p':
      if (pageq) {
	if (totpages == 0)
	  pc = 0;
	else
	  pc = (pages + 0.0) / ((totpages + 0.0) / 10000);
	pc1 = ((int)(pc + 0.5)) / 100;
	pc2 = ((int)(pc + 0.5)) % 100;
	if (aq != PREFORMATTED) {
	  if (type == 'o')
	    fprintf(outf, " ");
	  else if (type == 'O')
	    fprintf(outf, "(");
	}
	if (pc1 == 100) {
	  if (aq == PREFORMATTED)
	    fprintf(outf, "100.00");
	  else
	    fprintf(outf, "  100%%");
	}
	else if (pc1 > 0 || pc2 > 0) {
	  if (aq == PREFORMATTED)
	    fprintf(outf, "%d.%02d", pc1, pc2);
	  else
	    fprintf(outf, "%2d%c%02d%%", pc1, decpoint, pc2);
	}
	else if (aq == PREFORMATTED)
	  fprintf(outf, "0.00");
	else
	  fprintf(outf, "      ");
	if (aq == PREFORMATTED)
	  fprintf(outf, "%s", presep);
	else if (type == 'o')
	  fprintf(outf, " : ");
	else if (type == 'O')
	  fprintf(outf, "): ");
	else
	  fprintf(outf, ": ");
      }
      break;
    case 'B':
      if (byq) {
	if (aq == PREFORMATTED)
	  fprintf(outf, "%.0f", bytes);
	else if (bytes / bdivider > 0.5) {
	  if (type == 'o')
	    fprintf(outf, " ");
	  else if (type == 'O')
	    fprintf(outf, "(");
	  double3printf(outf, ROUND(bytes / bdivider), repsepchar,
			bfieldwidth);
	  if (type == 'o')
	    fprintf(outf, " ");
	  else if (type == 'O')
	    fprintf(outf, ")");
	}
	else for (k = 0; k < bfieldwidth + 2 * (type == 'o' || type == 'O');
		  k++)
	  fprintf(outf, " ");
	fprintf(outf, "%s", (aq == PREFORMATTED)?presep:": ");
      }
      break;
    case 'b':
      if (byq) {
	if (totbytes < 0.5)
	  pc = 0;
	else
	  pc = bytes / (totbytes / 10000);
	pc1 = ((int)(pc + 0.5)) / 100;    /* whole no. of %bytes */
	pc2 = ((int)(pc + 0.5)) % 100;    /* remaining 100ths. */
	if (aq != PREFORMATTED) {
	  if (type == 'o')
	    fprintf(outf, " ");
	  else if (type == 'O')
	    fprintf(outf, "(");
	}
	if (pc1 == 100) {
	  if (aq == PREFORMATTED)
	    fprintf(outf, "100.00");
	  else
	    fprintf(outf, "  100%%");
	}
	else if (pc1 > 0 || pc2 > 0) {
	  if (aq == PREFORMATTED)
	    fprintf(outf, "%d.%02d", pc1, pc2);
	  else
	    fprintf(outf, "%2d%c%02d%%", pc1, decpoint, pc2);
	}
	else if (aq == PREFORMATTED)
	  fprintf(outf, "0.00");
	else
	  fprintf(outf, "      ");
	if (aq == PREFORMATTED)
	  fprintf(outf, "%s", presep);
	else if (type == 'o')
	  fprintf(outf, " : ");
	else if (type == 'O')
	  fprintf(outf, "): ");
	else
	  fprintf(outf, ": ");
      }
      break;
    }
  }
}


/*** A nasty header bit. Return rough floor -- accurate if negative. ***/
/* (NB: good enough to use total_bytes in place of total_brow_bytes etc. */
int whatincluded(FILE *outf, int sortby, char *minreqstr, char *minpagestr,
		 char *minbytestr, char singular[27], char plural[29],
		 flag subdoms, char gender)
{
  extern double total_bytes;
  extern int total_succ_reqs, total_page_reqs;

  int genfloor;
  int tempint;
  char tempc;
  char *minstr;

  if (sortby == BYBYTES) {
    minstr = minbytestr;
    if (minstr[0] == '-')
      genfloor = (int)bytefloor(total_bytes, minbytestr);
    else
      genfloor = (int)(ceil(bytefloor(total_bytes, minbytestr)));
  }
  else if (sortby == BYPAGES) {
    minstr = minpagestr;
    genfloor = reqfloor(total_page_reqs, minpagestr);
  }
  else {
    minstr = minreqstr;
    genfloor = reqfloor(total_succ_reqs, minreqstr);
  }

  if (minstr[0] == '-') {
    if (genfloor == -1) {
      if (gender == 'm')
	fprintf(outf, lngstr[firstsm_], singular);
      else if (gender == 'f')
	fprintf(outf, lngstr[firstsf_], singular);
      else /* gender == 'n' */
	fprintf(outf, lngstr[firstsn_], singular);
    }
    else {
      if (gender == 'm')
	fprintf(outf, lngstr[firstdsm_], -genfloor, plural);
      else if (gender == 'f')
	fprintf(outf, lngstr[firstdsf_], -genfloor, plural);
      else /* gender == 'n' */
	fprintf(outf, lngstr[firstdsn_], -genfloor, plural);
    }
  }  /* end if minstr[0] == '-' */
  else {
    if (gender == 'm')
      fprintf(outf, lngstr[allsm_], plural);
    else if (gender == 'f')
      fprintf(outf, lngstr[allsf_], plural);
    else /* gender == 'n' */
      fprintf(outf, lngstr[allsn_], plural);
      
    if (genfloor > 0) {
      fprintf(outf, " %s ", lngstr[atleast_]);
      tempint = MAX((int)strlen(minstr) - 1, 0);
      if (minstr[tempint] == '%') {
	minstr[tempint] = '\0';
	doublefprintf(outf, atof(minstr));
	if (sortby == BYBYTES)
	  fprintf(outf, lngstr[ptraffic_]);
	else if (sortby == BYPAGES)
	  fprintf(outf, lngstr[ppages_]);
	else if (sortby == BYREQUESTS)
	  fprintf(outf, lngstr[prequests_]);
      }
      else if (sortby == BYBYTES) {
	if (minbytestr[tempint] == 'k' || minbytestr[tempint] == 'M' ||
	    minbytestr[tempint] == 'G' || minbytestr[tempint] == 'T') {
	  tempc = minbytestr[tempint];
	  minbytestr[tempint] = '\0';
	  doublefprintf(outf, atof(minbytestr));
	  fprintf(outf, " %c%s", tempc, lngstr[btraffic_]);
	}
	else {
	  doublefprintf(outf, atof(minbytestr));
	  fprintf(outf, " %s", lngstr[btraffic_]);
	}
      }
      else if (sortby == BYPAGES) {
	fprintf(outf, "%d %s", atoi(minpagestr),
		(atoi(minpagestr) == 1)?lngstr[pagereq_]:lngstr[pagereqs_]);
      }
      else { /* sortby not BYTES or PAGES */
	fprintf(outf, "%d %s", atoi(minreqstr),
		(atoi(minreqstr) == 1)?lngstr[request_]:lngstr[requests_]);
      }
    }    /* end if genfloor > 0 */
  }      /* end if minstr[0] != '-' */
  if (subdoms)
    fprintf(outf, ".\n");
  else if (sortby == BYBYTES || sortby == BYPAGES || sortby == BYREQUESTS) {
    if (gender == 'm')
      fprintf(outf, ",%s%s", (genfloor > 0)?"\n  ":" ",
	      (genfloor == -1)?lngstr[sortedms_]:lngstr[sortedmp_]);
    else if (gender == 'f')
      fprintf(outf, ",%s%s", (genfloor > 0)?"\n  ":" ",
	      (genfloor == -1)?lngstr[sortedfs_]:lngstr[sortedfp_]);
    else /* gender == 'n' */
      fprintf(outf, ",%s%s", (genfloor > 0)?"\n  ":" ",
	      (genfloor == -1)?lngstr[sortedns_]:lngstr[sortednp_]);
    if (sortby == BYBYTES)
      fprintf(outf, " %s.\n", lngstr[traffic_]);
    else if (sortby == BYPAGES)
      fprintf(outf, " %s.\n", lngstr[npages_]);
    else /* sortby == BYREQUESTS */
      fprintf(outf, " %s.\n", lngstr[nrequests_]);
  }
  else if (sortby == ALPHABETICAL) {
    if (gender == 'm')
      fprintf(outf, ",%s%s.\n", (genfloor > 0)?"\n  ":" ",
	      (genfloor == -1)?lngstr[alphasortms_]:lngstr[alphasortmp_]);
    else if (gender == 'f')
      fprintf(outf, ",%s%s.\n", (genfloor > 0)?"\n  ":" ",
	      (genfloor == -1)?lngstr[alphasortfs_]:lngstr[alphasortfp_]);
    else /* gender == 'n' */
      fprintf(outf, ",%s%s.\n", (genfloor > 0)?"\n  ":" ",
	      (genfloor == -1)?lngstr[alphasortns_]:lngstr[alphasortnp_]);
  }
  else {  /* unsorted */
    if (gender == 'm')
      fprintf(outf, ", %s.\n",
	      (genfloor == -1)?lngstr[unsortedms_]:lngstr[unsortedmp_]);
    else if (gender == 'f')
      fprintf(outf, ", %s.\n",
	      (genfloor == -1)?lngstr[unsortedfs_]:lngstr[unsortedfp_]);
    else /* gender == 'n' */
      fprintf(outf, ", %s.\n",
	      (genfloor == -1)?lngstr[unsortedns_]:lngstr[unsortednp_]);
  }

  return(genfloor);

}

/* The same without the printing (just to return the right value). */

int whatincludednop(int sortby, char *minreqstr, char *minpagestr,
		    char *minbytestr)
{
  extern double total_bytes;
  extern int total_succ_reqs, total_page_reqs;

  int genfloor;

  if (sortby == BYBYTES) {
    if (minbytestr[0] == '-')
      genfloor = (int)bytefloor(total_bytes, minbytestr);
    else
      genfloor = (int)(ceil(bytefloor(total_bytes, minbytestr)));
  }
  else if (sortby == BYPAGES)
    genfloor = reqfloor(total_page_reqs, minpagestr);
  else
    genfloor = reqfloor(total_succ_reqs, minreqstr);

  return(genfloor);

}

/*** Generic output function for generic objects ***/

void genout(FILE *outf, struct genstruct *sorthead, int tot_reqs,
	    int tot_pages, double tot_bytes, int sortby, char *minreqstr,
	    char *minpagestr, char *minbytestr, int max_reqs, int max_pages,
	    double max_bytes, char *wantcols, char anchor[10], char title[36],
	    char singular[22], char plural[24], char colhead[24], char gender,
	    char codeletter, flag alphahost, flag byq, flag pageq,
	    struct include *links, struct alias *aka,
	    char baseurl[MAXSTRINGLENGTH])
{
  extern int pagewidth;
  extern int dirlevel;
  extern int Smaxlength;
  extern int aq;
  extern flag rawbytes;
  extern char repsepchar;

  struct genstruct *p;
  int fieldwidth, pfieldwidth, bfieldwidth, graphwidth;
  int genfloor;
  double bdivider;
  char bprefix[2];
  char akaname[MAXSTRINGLENGTH];
  char *cols;
  int i, j, tempint;
  char *tempc;
  
  bprefix[0] = '\0';
  bprefix[1] = '\0';

  if (aq != PREFORMATTED) {

    if (!aq) {
      fprintf(outf, "\n\n<hr>\n<h2><a NAME=\"%s\">%s</a></h2>\n\n", anchor,
	      title);
      gotos(outf, codeletter);
      fprintf(outf, "<p>");
    }
    else {
      fprintf(outf, "%s\n", title);
      for (tempc = title; *tempc != '\0'; tempc++)
	fprintf(outf, "-");
      fprintf(outf, "\n");
    }
    
    genfloor = whatincluded(outf, sortby, minreqstr, minpagestr, minbytestr,
			    singular, plural, FALSE, gender);
    if (codeletter == 'i') {
      if (!aq)
	fprintf(outf, "<br>");
      fprintf(outf, lngstr[dirlevel_], dirlevel);
      fprintf(outf, ".\n");
    }

    if (aq)
      fprintf(outf, "\n");
    else
      fprintf(outf, "<pre>");

    tempint = 10000;
    for (fieldwidth = 5; max_reqs / tempint >= 10; fieldwidth++)
      tempint *= 10;
    if (repsepchar != '\0' && max_reqs >= 10000)
      fieldwidth = fieldwidth + ((fieldwidth - 1) / 3);

    if (pageq) {
      tempint = 10000;
      for (pfieldwidth = 5; max_pages / tempint >= 10; pfieldwidth++)
	tempint *= 10;
      if (repsepchar != '\0' && max_pages >= 10000)
	pfieldwidth = pfieldwidth + ((pfieldwidth - 1) / 3);
    }

    if (byq) {
      bdivider = finddivider(max_bytes, bprefix);
      if (rawbytes) {
	tempint = 100000;
	for (bfieldwidth = 6; max_bytes / tempint >= 10; bfieldwidth++)
	  tempint *= 10;
      }
      else
	bfieldwidth = 6;
      if (repsepchar != '\0' && max_bytes / bdivider >= 99999.5)
	bfieldwidth = bfieldwidth + ((bfieldwidth - 1) / 3);
    }

    printcolheads(outf, wantcols, fieldwidth, pfieldwidth, bfieldwidth,
		  bprefix, colhead, 0, codeletter, byq, pageq, FALSE);

    if (alphahost) {
      graphwidth = pagewidth;
      for (cols = wantcols; *cols != '\0'; cols++) {
	switch(*cols) {
	case 'R':
	  graphwidth -= fieldwidth + 2;
	  break;
	case 'P':
	  graphwidth -= pfieldwidth + 2;
	  break;
	case 'B':
	  graphwidth -= bfieldwidth + 2;
	  break;
	case 'r':
	case 'p':
	case 'b':
	  graphwidth -= 8;
	  break;
	}
      }
      graphwidth = MIN(graphwidth, Smaxlength);
    }
  }

  else /* aq == PREFORMATTED */
    genfloor = whatincludednop(sortby, minreqstr, minpagestr, minbytestr);

  if (genfloor < 0)
    j = genfloor;
  else j = 1;

  for(p = sorthead; p -> name != NULL && (j++) != 0;
      p = p -> next) {

    if (aq == PREFORMATTED)
      precols(outf, wantcols, codeletter, byq, pageq);

    printcols(outf, wantcols, p -> reqs, p -> pages, p -> bytes, fieldwidth,
	      pfieldwidth, bfieldwidth, bdivider, tot_reqs, tot_pages,
	      tot_bytes, codeletter, byq, pageq);

    if (alphahost && !isdigit(p -> name[0])) {  /* we've swapped the names */
      reversehostname(p -> name);
      strcpy(akaname, p -> name);
      if (aka != NULL)
	doaliaslist(akaname, aka);
      /* Also in that case right align names */
      if (aq != PREFORMATTED) {
	for (i = graphwidth - (int)strlen(akaname); i > 0; i--)
	  fprintf(outf, " ");
      }
    }
    else {
      strcpy(akaname, p -> name);
      if (aka != NULL)
	doaliaslist(akaname, aka);
    }

    if (links != NULL && included(p -> name, p -> ispage, links)) {
      fprintf(outf, "<a HREF=\"");
      htmlfprintf(outf, baseurl);
      htmlfprintf(outf, p -> name);
      fprintf(outf, "\">");
      htmlfprintf(outf, akaname);
      fprintf(outf, "</a>");
    }
    else   /* (the usual case for most reports) */
      if (aq == HTML)
	htmlfprintf(outf, akaname);
      else
	fprintf(outf, "%s", akaname);
    fprintf(outf, "\n");

  }
      
  if (aq == ASCII)
    asciiline(outf);
  else if (aq == HTML)
    fprintf(outf, "</pre>");
	
}

/*** The domain report is similar to the generic ones. It differs in that
     the domains are stored in a different structure, and that subdomains
     must be printed. ***/

void domout(FILE *outf, int firstdom)
{
  extern struct domain **ohead;
  extern int aq;
  extern flag byq, rawbytes;
  extern int osortby;
  extern char *ominbytestr, *ominpagestr, *ominreqstr;
  extern char *Ominbytestr, *Ominpagestr, *Ominreqstr;
  extern int omaxreqs, omaxpages;
  extern double omaxbytes;
  extern int Onumber;
  extern char ocols[];
  extern double total_bytes;
  extern int total_succ_reqs, total_page_reqs;
  extern char *presep;
  extern char repsepchar;

  int ofloor;

  struct domain *p;
  double bdivider;
  char bprefix[2];
  int fieldwidth, pfieldwidth, bfieldwidth;
  int i, j, tempint;
  char *tempp;

  bprefix[0] = '\0';
  bprefix[1] = '\0';

  if (aq != PREFORMATTED) {
    if (!aq) {
      fprintf(outf,
	      "\n\n<hr>\n<h2><a NAME=\"Domain\">%s</a></h2>\n\n",
	      lngstr[domrep_]);
      gotos(outf, 'o');
    }
    else {
      fprintf(outf, "%s\n", lngstr[domrep_]);
      matchlength(outf, lngstr[domrep_], '-');
      fprintf(outf, "\n");
    }
  
    if (!aq)
      fprintf(outf, "<p>");

    ofloor = whatincluded(outf, osortby, ominreqstr, ominpagestr, ominbytestr,
			  lngstr[domgs_], lngstr[domgp_], FALSE,
			  lngstr[domgen_][0]);
    if (Onumber > 0) {
      if (!aq)
	fprintf(outf, "<br>");
      whatincluded(outf, osortby, Ominreqstr, Ominpagestr, Ominbytestr,
		   lngstr[subdomgs_], lngstr[subdomgp_], TRUE,
		   lngstr[subdomgen_][0]);
    }

    if (aq)
      fprintf(outf, "\n");
    else
      fprintf(outf, "<pre>");
  
    tempint = 10000;
    for (fieldwidth = 5; omaxreqs / tempint >= 10; fieldwidth++)
      tempint *= 10;
    if (repsepchar != '\0' && omaxreqs >= 10000)
      fieldwidth = fieldwidth + ((fieldwidth - 1) / 3);

    tempint = 10000;
    for (pfieldwidth = 5; omaxpages / tempint >= 10; pfieldwidth++)
      tempint *= 10;
    if (repsepchar != '\0' && omaxpages >= 10000)
      pfieldwidth = pfieldwidth + ((pfieldwidth - 1) / 3);
  
    if (byq) {
      bdivider = finddivider(omaxbytes, bprefix);
      if (rawbytes) {
	tempint = 100000;
	for (bfieldwidth = 6; omaxbytes / tempint >= 10; bfieldwidth++)
	  tempint *= 10;
      }
      else
	bfieldwidth = 6;
      if (repsepchar != '\0' && omaxbytes / bdivider >= 99999.5)
	bfieldwidth = bfieldwidth + ((bfieldwidth - 1) / 3);
    }
  
    printcolheads(outf, ocols, fieldwidth, pfieldwidth, bfieldwidth, bprefix,
		  lngstr[dom_], 0, (Onumber > 0)?'o':'\0', byq, ON, FALSE);

  }

  else /* aq == PREFORMATTED */
    ofloor = whatincludednop(osortby, ominreqstr, ominpagestr, ominbytestr);

  if (ofloor < 0)
    j = ofloor;    
  else j = 1;

  for (i = firstdom; i >= 0 && (j++) != 0; i = ohead[i] -> nexti) {

    if (!(i == DOMHASHSIZE - 2 && ohead[i] -> reqs == -1)) {

      if (aq == PREFORMATTED)
	precols(outf, ocols, 'o', byq, ON);

      printcols(outf, ocols, ohead[i] -> reqs, ohead[i] -> pages,
		ohead[i] -> bytes, fieldwidth, pfieldwidth, bfieldwidth,
		bdivider, total_succ_reqs, total_page_reqs, total_bytes,
		(Onumber > 0)?'o':'\0', byq, ON);
      
      if (ohead[i] -> id[0] == '*')
	/* flagged domains, not real domain names */
	fprintf(outf, "[%s]\n", ohead[i] -> name);
      else if (ohead[i] -> name[0] == '?')
	/* real domain, but don't print name */
	fprintf(outf, ".%s\n", ohead[i] -> id);
      else if (aq == PREFORMATTED)
	fprintf(outf, ".%s%s%s\n", ohead[i] -> id, presep, ohead[i] -> name);
      else if (aq == ASCII)
	fprintf(outf, ".%s (%s)\n", ohead[i] -> id, ohead[i] -> name);
      else {  /* aq == HTML */
	fprintf(outf, ".%s (", ohead[i] -> id);
	htmlfprintf(outf, ohead[i] -> name);
	fprintf(outf, ")\n");
      }
      
      /* Now print its subdomains too. */
      
      for (p = ohead[i] -> next; p -> name != NULL;
	   p = p -> next) {

	if (aq == PREFORMATTED)
	  precols(outf, ocols, 'O', byq, ON);

	printcols(outf, ocols, p -> reqs, p -> pages, p -> bytes, fieldwidth,
		  pfieldwidth, bfieldwidth, bdivider, total_succ_reqs,
		  total_page_reqs, total_bytes, 'O', byq, ON);

	if (aq != PREFORMATTED) {
	  tempp = p -> id;
	  while ((tempp = strchr(tempp, '.')) != NULL) {
	    fprintf(outf, "  "); 
	    /* print two spaces for each dot in name */
	    tempp++;
	  }
	  if (i == DOMHASHSIZE - 1)
	    fprintf(outf, "  ");  /* + 2 more for numerical domains */
	}
	
	fprintf(outf, "%s", p -> id);
	
	if (p -> name[0] != '?') {   /* print name */
	  if (aq == PREFORMATTED)
	    fprintf(outf,"%s%s", presep, p -> name);
	  else if (aq == ASCII)
	    fprintf(outf, " (%s)", p -> name);
	  else {
	    fprintf(outf, " (");
	    htmlfprintf(outf, p -> name);
	    fprintf(outf, ")");
	  }
	}
	
	fprintf(outf, "\n");
	
      }    /* end for domp */
	
    }

  }   /* end for (i = running over domains) */
    
  if (aq == ASCII)
    asciiline(outf);
  else if (aq == HTML)
    fprintf(outf, "</pre>");
	
}

/*** The date reports aren't quite generic enough to combine completely,
     but we can go a long way towards it. ***/
/*** First a function for printing out the headers of a report and finding
     the fieldwidths etc.; then one for printing out each individual line. ***/

void datehead(FILE *outf, int maxreq, int maxpages, double maxbytes,
	      char *wantcols, char *graphtype, char anchor[11],
	      char title[31], char colhead[13], int colwidth,
	      char codeletter, int *unit, int *fieldwidth, int *pfieldwidth,
	      int *bfieldwidth, int *graphwidth, double *bdivider)
     /* assumes colwidth >= strlen(colhead) */
     /* The last 5 args are returned altered */
{
  extern int aq;
  extern flag byq, rawbytes, graphical;
  extern int pagewidth;
  extern char *imagedir;
  extern char markchar, sepchar, repsepchar;

  char *cols;
  char bprefix[2];
  int i, j, tempint;
  char *tempc;

  bprefix[0] = '\0';
  bprefix[1] = '\0';

  if (*graphtype == 'b')
    *graphtype = 'B';
  if (*graphtype == 'p')
    *graphtype = 'P';

  if (!aq) {
    fprintf(outf, "<hr>\n<h2><a NAME=\"%s\">%s</a></h2>\n", anchor, title);
	  gotos(outf, codeletter);
  }
  else {
    fprintf(outf, "%s\n", title);
    for (tempc = title; *tempc != '\0'; tempc++)
      fprintf(outf, "-");
    fprintf(outf, "\n");
  }
    
  tempint = 10000;
  for (*fieldwidth = 5; maxreq / tempint >= 10; (*fieldwidth)++)
    tempint *= 10;   /* so fieldwidth is log_10(maxreq), but >= 5 */
  if (repsepchar != '\0' && maxreq >= 10000)
    *fieldwidth = *fieldwidth + ((*fieldwidth - 1) / 3);

  tempint = 10000;
  for (*pfieldwidth = 5; maxpages / tempint >= 10; (*pfieldwidth)++)
    tempint *= 10;
  if (repsepchar != '\0' && maxpages >= 10000)
    *pfieldwidth = *pfieldwidth + ((*pfieldwidth - 1) / 3);
	
  if (byq) {
    *bdivider = finddivider(maxbytes, bprefix);
    if (rawbytes || (*graphtype == 'B' && *unit > 0)) {
      tempint = 100000;
      for (*bfieldwidth = 6; maxbytes / tempint >= 10; (*bfieldwidth)++)
	tempint *= 10;
    }
    else
      *bfieldwidth = 6;
    if (repsepchar != '\0' && maxbytes / *bdivider >= 99999.5)
      *bfieldwidth = *bfieldwidth + ((*bfieldwidth - 1) / 3);
  }

  if (*unit <= 0) {   /* (o/wise just use the given amount) */

    /* Calculate the graphwidth */
    *graphwidth = pagewidth - colwidth - 2;
    for (cols = wantcols; *cols != '\0'; cols++) {
      switch(*cols) {
      case 'R':
	*graphwidth -= *fieldwidth + 2;
	break;
      case 'P':
	*graphwidth -= *pfieldwidth + 2;
	break;
      case 'B':
	*graphwidth -= *bfieldwidth + 2;
	break;
      case 'r':
      case 'p':
      case 'b':
	*graphwidth -= 8;
	break;
      }
    }
    *graphwidth = MAX(*graphwidth, MINGRAPHWIDTH);  /* must be >= MGW wide */
	                                  
    if (*graphtype == 'B')
      *unit = (maxbytes - 1) / (*bdivider * *graphwidth);
    else if (*graphtype == 'P')
      *unit = (maxpages - 1) / *graphwidth;
    else   /* graphtype assumed to be 'R' */
      *unit = (maxreq - 1) / *graphwidth;
                   	        /* except we want a 'nice' amount, so ... */
	             /* (Nice amount is 1, 1.5, 2, 2.5, 3, 4, 5, 6, 8 * 10^n */

    j = 0;
    while (*unit > 24) {
      *unit /= 10;
      j++;
    }
    if (*unit == 6)
      *unit = 7;
    else if (*unit == 8)
      *unit = 9;
    else if (*unit >= 20)
      *unit = 24;
    else if (*unit >= 15)
      *unit = 19;
    else if (*unit >= 10)
      *unit = 14;
    (*unit)++;
    for (i = 0; i < j; i++) {
      *unit *= 10;
    }

  }     /* end if (*unit <= 0) */

  else if (*graphtype == 'B') {   /* o/wise unit doesn't make sense */
    *bdivider = 1;
    bprefix[0] = '\0';
  }

  if (!aq)
    fprintf(outf, "\n<p>");
  if (aq == HTML && graphical) {
    fprintf(outf, "%s (<tt><img src=\"", lngstr[eachunit_]);
    htmlfprintf(outf, imagedir);
    fprintf(outf, "bar1.gif\" alt=\"");
    htmlputc(markchar, outf);
    fprintf(outf, "\"></tt>) %s ", lngstr[represents_]);
    int3printf(outf, *unit, sepchar, 0);
    if (*graphtype == 'B')
      fprintf(outf, " %s%s, %s.\n\n", bprefix,
	      (*unit == 1)?lngstr[byte_]:lngstr[bytes_], lngstr[partof_]);
    else if (*graphtype == 'P') {
      if (*unit == 1)
	fprintf(outf, " %s.\n\n", lngstr[pagereq_]);
      else
	fprintf(outf, " %s, %s.\n\n", lngstr[pagereqs_], lngstr[partof_]);
    }
    else {
      if (*unit == 1)
	fprintf(outf, " %s.\n\n", lngstr[request_]);
      else
	fprintf(outf, " %s, %s.\n\n", lngstr[requests_], lngstr[partof_]);
    }
  }
  else {
    fprintf(outf, "\n%s (%c) %s ", lngstr[eachunit_], markchar,
	    lngstr[represents_]);
    int3printf(outf, *unit, sepchar, 0);
    if (*graphtype == 'B')
      fprintf(outf, " %s%s, %s.\n\n", bprefix,
	      (*unit == 1)?lngstr[byte_]:lngstr[bytes_], lngstr[partof_]);
    else if (*graphtype == 'P') {
      if (*unit == 1)
	fprintf(outf, " %s.\n\n", lngstr[pagereq_]);
      else
	fprintf(outf, " %s, %s.\n\n", lngstr[pagereqs_], lngstr[partof_]);
    }
    else {
      if (*unit == 1)
	fprintf(outf, " %s.\n\n", lngstr[request_]);
      else
	fprintf(outf, " %s, %s.\n\n", lngstr[requests_], lngstr[partof_]);
    }
  }
  if (!aq)
    fprintf(outf, "<pre width=%d><tt>\n", pagewidth);
	
  printcolheads(outf, wantcols, *fieldwidth, *pfieldwidth, *bfieldwidth,
		bprefix, colhead, colwidth, codeletter, byq, ON, TRUE);

}

/* As promised, each separate line. We print name of date in output() though */

void dateline(FILE *outf, int reqs, int pages, double bytes, char *wantcols,
	      char graphtype, int fieldwidth, int pfieldwidth, int bfieldwidth,
	      int unit, double bdivider)
{
  extern double total_bytes;
  extern int total_succ_reqs, total_page_reqs;
  extern flag aq, byq;

  printcols(outf, wantcols, reqs, pages, bytes, fieldwidth, pfieldwidth,
	    bfieldwidth, bdivider, total_succ_reqs, total_page_reqs,
	    total_bytes, '\0', byq, ON);

  if (aq != PREFORMATTED) {
    if (graphtype == 'B')
      barplot(outf, (int)(ceil(bytes / (unit * bdivider))));
    else if (graphtype == 'P')
      barplot(outf, (pages == 0)?0:((pages - 1) / unit) + 1);
    else
      barplot(outf, (reqs == 0)?0:((reqs - 1) / unit) + 1);
    fprintf(outf, "\n");  /* PREFORMATTED has more before the \n */
  }
}

/*** The status code report (very simple) ***/

void statusout(FILE *outf)
{
  extern int status[], statusnos[];
  extern char statusstrs[NO_STATUS][MAXSTATUSLENGTH];
  extern char ccols[];
  extern int aq;
  extern char repsepchar;

  int fieldwidth;
  int maxreqs = 0;
  char tempstr[MAXSTRINGLENGTH];
  int i;

  if (aq != PREFORMATTED) {
    for (i = 0; i < NO_STATUS; i++)
      maxreqs = MAX(maxreqs, status[i]);

    if (aq == HTML) {
      fprintf(outf, "\n\n<hr>\n<h2><a NAME=\"Status\">%s</a></h2>\n\n",
	      lngstr[statrep_]);
      gotos(outf, 'c');
      fprintf(outf, "<pre>");
    }
    else {
      fprintf(outf, "%s\n", lngstr[statrep_]);
      matchlength(outf, lngstr[statrep_], '-');
      fprintf(outf, "\n\n");
    }
    i = 10000;
    for (fieldwidth = 5; maxreqs / i >= 10; fieldwidth++)
      i *= 10;
    if (repsepchar != '\0' && maxreqs > 10000)
      fieldwidth = fieldwidth + ((fieldwidth - 1) / 3);

    sprintf(tempstr, "%s %s", lngstr[nr_], lngstr[description_]);
    printcolheads(outf, ccols, fieldwidth, 0, 0, "", tempstr, 0, 'c', FALSE,
		  FALSE, FALSE);
  }

  for (i = 0; i < NO_STATUS; i++) {
    if (status[i] > 0) {
      if (aq == PREFORMATTED)
	precols(outf, ccols, 'c', OFF, OFF);
      printcols(outf, ccols, status[i], 0, 0.0, fieldwidth, 0, 0, 0.0, 1, 1,
		1.0, 'c', OFF, OFF);
      if (aq == PREFORMATTED)
	fprintf(outf, "%d\n", statusnos[i]);
      else if (statusstrs[i][0] == '[')
	fprintf(outf, "    %s\n", statusstrs[i]);
      else
	fprintf(outf, "%d %s\n", statusnos[i], statusstrs[i]);
    }
  }

  if (aq == ASCII)
    asciiline(outf);
  else if (aq == HTML)
    fprintf(outf, "</pre>");
}

/*** The error report ***/

void errout(FILE *outf, int errorder[NO_ERRS])
{
  extern int errors[NO_ERRS];
  extern char errs[NO_ERRS][MAXERRLENGTH];
  extern int eminreqs;
  extern char ecols[];
  extern int aq;
  extern char repsepchar;

  int fieldwidth;
  int i;

  if (aq != PREFORMATTED) {
    if (aq == HTML) {
      fprintf(outf, "\n\n<hr>\n<h2><a NAME=\"Error\">%s</a></h2>\n\n",
	      lngstr[errrep_]);
      gotos(outf, 'e');
      fprintf(outf, "<p>");
    }
    else {
      fprintf(outf, "%s\n", lngstr[errrep_]);
      matchlength(outf, lngstr[errrep_], '-');
      fprintf(outf, "\n");
    }

    if (eminreqs == 0)
      fprintf(outf, "%s, ", lngstr[allerrs_]);
    else if (eminreqs == 1)
      fprintf(outf, "%s,\n  ", lngstr[allerrs1_]);
    else {
      fprintf(outf, lngstr[allerrsn_], eminreqs);
      fprintf(outf, "\n  ");
    }
    fprintf(outf, "%s.", lngstr[occsort_]);
  
    if (aq)
      fprintf(outf, "\n\n");
    else
      fprintf(outf, "<pre>");

    i = 10000;
    for (fieldwidth = 5; errors[errorder[0]] / i >= 10; fieldwidth++)
      i *= 10;
    if (repsepchar != '\0' && errors[errorder[0]] >= 10000)
      fieldwidth = fieldwidth + ((fieldwidth - 1) / 3);

    printcolheads(outf, ecols, fieldwidth, 0, 0, "", lngstr[errtype_], 0, 'e',
		  FALSE, FALSE, FALSE);
  }

  for (i = 0; i < NO_ERRS && errors[errorder[i]] >= eminreqs; i++) {
    if (aq == PREFORMATTED)
      precols(outf, ecols, 'e', FALSE, FALSE);
    printcols(outf, ecols, errors[errorder[i]], 0, 0.0, fieldwidth, 0, 0, 0.0,
	      1, 1, 1.0, 'e', OFF, OFF);
    fprintf(outf, "%s\n",
	    (errs[errorder[i]][0] == '\0')?"[unknown]":errs[errorder[i]]);
  }

  if (aq == ASCII)
    asciiline(outf);
  else if (aq == HTML)
    fprintf(outf, "</pre>");
}

/*** And the general summary ***/

void gensum(FILE *outf)
{
  extern int corrupt_lines, other_lines;
  extern int no_urls, no_hosts, no_urls7, no_hosts7, no_new_hosts7;
  extern double total_bytes, total_bytes7;
  extern int total_succ_reqs, total_fail_reqs, total_other_reqs;
  extern int total_succ_reqs7, total_fail_reqs7, total_other_reqs7;
  extern int total_page_reqs, total_page_reqs7;
  extern flag mq, Wq, dq, Dq, hq, oq, Sq, iq, rq, q7, byq;
  extern int sq, aq;
  extern char sepchar, *presep;
  extern struct timestruct starttimec, totime, firsttime, lasttime, oldtime;
  extern char dayname[7][11];
  extern char monthname[12][12];

  int totalmins;    /* between first and last entries analysed */
  double bdivider;
  char bprefix[2];  /* kilo, Mega, etc. */

  bprefix[0] = '\0';
  bprefix[1] = '\0';

  if (aq == HTML)
    fprintf(outf, "<hr>");

  if (aq == PREFORMATTED)
    fprintf(outf, "\nx%sPS%s%d%s%d%s%d%s%d%s%d", presep,
	    presep, starttimec.year, presep, starttimec.monthno + 1,
	    presep, starttimec.date, presep, starttimec.hr, presep,
	    starttimec.min);
  else {
    fprintf(outf, "\n%s ", lngstr[progstart_]);
    dateprintf(outf, lngstr[datefmt2_], starttimec.date, starttimec.monthno,
	       starttimec.year, starttimec.hr, starttimec.min);
    fprintf(outf, " %s.\n", lngstr[localtime_]);
  }

  if (firsttime.code > oldtime.code)
    q7 = OFF;

  if (total_succ_reqs > 0) {
    totalmins = minsbetween(firsttime.date, firsttime.monthno,
			    firsttime.year, firsttime.hr, firsttime.min,
			    lasttime.date, lasttime.monthno, lasttime.year,
			    lasttime.hr, lasttime.min) + 1;
    if (aq == HTML)
      fprintf(outf, "<br>");
    if (aq == PREFORMATTED) {
      fprintf(outf, "\nx%sFR%s%d%s%d%s%d%s%d%s%d", presep, presep,
	      firsttime.year, presep, firsttime.monthno + 1, presep,
	      firsttime.date, presep, firsttime.hr, presep, firsttime.min);
      fprintf(outf, "\nx%sLR%s%d%s%d%s%d%s%d%s%d", presep, presep,
	      lasttime.year, presep, lasttime.monthno + 1, presep,
	      lasttime.date, presep, lasttime.hr, presep, lasttime.min);
      if (q7)
	fprintf(outf, "\nx%sL7%s%d%s%d%s%d%s%d%s%d", presep, presep,
		oldtime.year, presep, oldtime.monthno + 1, presep,
		oldtime.date, presep, oldtime.hr, presep, oldtime.min);
    }
    else {
      fprintf(outf, "%s ", lngstr[reqstart_]);
      dateprintf(outf, lngstr[datefmt2_], firsttime.date, firsttime.monthno,
		 firsttime.year, firsttime.hr, firsttime.min);
      fprintf(outf, " %s\n  ", lngstr[to_]);
      dateprintf(outf, lngstr[datefmt2_], lasttime.date, lasttime.monthno,
		 lasttime.year, lasttime.hr, lasttime.min);
      fprintf(outf, " (%.1f %s).\n\n", ((double)totalmins) / 1440.0,
	      lngstr[days_]);
    }
  }

  if (aq == HTML)
    fprintf(outf, "<p><b>%s%s</b> ", lngstr[succreqs_], lngstr[colon_]);
  else if (aq == ASCII)
    fprintf(outf, "%s%s ", lngstr[succreqs_], lngstr[colon_]);
  else   /* aq == PREFORMATTED */
    fprintf(outf, "\nx%sSR%s", presep, presep);
  int3printf(outf, total_succ_reqs, sepchar, 0);
  if (q7) {
    if (aq == PREFORMATTED)
      fprintf(outf, "\nx%sS7%s%d", presep, presep, total_succ_reqs7);
    else {
      fprintf(outf, " (");
      int3printf(outf, total_succ_reqs7, sepchar, 0);
      fprintf(outf, ")");
    }
  }
  if (totalmins > 30 && aq != PREFORMATTED) {
    if (aq == HTML)
      fprintf(outf, "\n<br><b>%s%s</b> ", lngstr[avereqs_], lngstr[colon_]);
    else
      fprintf(outf, "\n%s%s ", lngstr[avereqs_], lngstr[colon_]);
      
    if (total_succ_reqs < 2)
      fprintf(outf, "0");
    else
      double3printf(outf, ROUND((double)(total_succ_reqs - 1)) * 1440.0 / (totalmins + 0.0), sepchar, 0);
    if (q7) {
      fprintf(outf, " (");
      int3printf(outf, total_succ_reqs7 / 7, sepchar, 0);
      fprintf(outf, ")");
    }
  }
  if (total_page_reqs > 0) {
    if (aq == HTML)
      fprintf(outf, "\n<br><b>%s%s</b> ", lngstr[totpages_], lngstr[colon_]);
    else if (aq == ASCII)
      fprintf(outf, "\n%s%s ", lngstr[totpages_], lngstr[colon_]);
    else   /* aq == PREFORMATTED */
      fprintf(outf, "\nx%sPR%s", presep, presep);
    int3printf(outf, total_page_reqs, sepchar, 0);
    if (q7) {
      if (aq == PREFORMATTED)
	fprintf(outf, "\nx%sP7%s%d", presep, presep, total_page_reqs7);
      else {
	fprintf(outf, " (");
	int3printf(outf, total_page_reqs7, sepchar, 0);
	fprintf(outf, ")");
      }
    }
    if (totalmins > 30 && aq != PREFORMATTED) {
      if (aq == HTML)
	fprintf(outf, "\n<br><b>%s%s</b> ", lngstr[avepages_], lngstr[colon_]);
      else
	fprintf(outf, "\n%s%s ", lngstr[avepages_], lngstr[colon_]);
      if (total_page_reqs < 2)
	fprintf(outf, "0");
      else
	double3printf(outf, ROUND((double)(total_page_reqs - 1)) * 1440.0 / (totalmins + 0.0), sepchar, 0);
      if (q7) {
	fprintf(outf, " (");
	int3printf(outf, total_page_reqs7 / 7, sepchar, 0);
	fprintf(outf, ")");
      }
    }
  }
  if (total_fail_reqs > 0) {
    if (aq == HTML)
      fprintf(outf, "\n<br><b>%s%s</b> ", lngstr[totfails_], lngstr[colon_]);
    else if (aq == ASCII)
      fprintf(outf, "\n%s%s ", lngstr[totfails_], lngstr[colon_]);
    else   /* aq == PREFORMATTED */
      fprintf(outf, "\nx%sFL%s", presep, presep);
    int3printf(outf, total_fail_reqs, sepchar, 0);
    if (q7) {
      if (aq == PREFORMATTED)
	fprintf(outf, "\nx%sF7%s%d", presep, presep, total_fail_reqs7);
      else {
	fprintf(outf, " (");
	int3printf(outf, total_fail_reqs7, sepchar, 0);
	fprintf(outf, ")");
      }
    }
  }
  if (total_other_reqs > 0) {
    if (aq == HTML)
      fprintf(outf, "\n<br><b>%s%s</b> ", lngstr[totredirs_], lngstr[colon_]);
    else if (aq == ASCII)
      fprintf(outf, "\n%s%s ", lngstr[totredirs_], lngstr[colon_]);
    else   /* aq == PREFORMATTED */
      fprintf(outf, "\nx%sRR%s", presep, presep);
    int3printf(outf, total_other_reqs, sepchar, 0);
    if (q7) {
      if (aq == PREFORMATTED)
	fprintf(outf, "\nx%sR7%s%d", presep, presep, total_other_reqs7);
      else {
	fprintf(outf, " (");
	int3printf(outf, total_other_reqs7, sepchar, 0);
	fprintf(outf, ")");
      }
    }
  }
  if (rq) {   /* These data are not collected o/wise */
    if (aq == HTML)
      fprintf(outf, "\n<br><b>%s%s</b> ", lngstr[distfiles_], lngstr[colon_]);
    else if (aq == ASCII)
      fprintf(outf, "\n%s%s ", lngstr[distfiles_], lngstr[colon_]);
    else   /* aq == PREFORMATTED */
      fprintf(outf, "\nx%sNF%s", presep, presep);
    int3printf(outf, no_urls, sepchar, 0);
    if (q7) {
      if (aq == PREFORMATTED)
	fprintf(outf, "\nx%sN7%s%d", presep, presep, no_urls7);
      else {
	fprintf(outf, " (");
	int3printf(outf, no_urls7, sepchar, 0);
	fprintf(outf, ")");
      }
    }
  }
  if ((sq == ON || sq == APPROX) && no_hosts > 0) {
    if (aq == HTML)
      fprintf(outf, "\n<br><b>%s%s</b> ",
	      (sq == ON)?lngstr[disthosts_]:lngstr[approxhosts_],
	      lngstr[colon_]);
    else if (aq == ASCII)
      fprintf(outf, "\n%s%s ",
	      (sq == ON)?lngstr[disthosts_]:lngstr[approxhosts_],
	      lngstr[colon_]);
    else   /* aq == PREFORMATTED */
      fprintf(outf, "\nx%s%cH%s", presep, (sq == ON)?'N':'A', presep);
    int3printf(outf, no_hosts, sepchar, 0);
    if (q7) {
      if (aq == PREFORMATTED)
	fprintf(outf, "\nx%s%c7%s%d", presep, (sq == ON)?'H':'A', presep,
		no_hosts7);
      else {
	fprintf(outf, " (");
	int3printf(outf, no_hosts7, sepchar, 0);
	fprintf(outf, ")");
      }
      if (aq == HTML)
	fprintf(outf, "\n<br><b>%s%s</b> ",
		(sq == ON)?lngstr[newhosts_]:lngstr[approxnewhosts_],
		lngstr[colon_]);
      else if (aq == ASCII)
	fprintf(outf, "\n%s%s ",
		(sq == ON)?lngstr[newhosts_]:lngstr[approxnewhosts_],
		lngstr[colon_]);
      else   /* aq == PREFORMATTED */
	fprintf(outf, "\nx%s%cV%s", presep, (sq == ON)?'N':'A', presep);
      int3printf(outf, no_new_hosts7, sepchar, 0);
    }
  }
  if (corrupt_lines > 0) {
    if (aq == HTML)
      fprintf(outf, "\n<br><b>%s%s</b> ", lngstr[corrupt_], lngstr[colon_]);
    else if (aq == ASCII)
      fprintf(outf, "\n%s%s ", lngstr[corrupt_], lngstr[colon_]);
    else   /* aq == PREFORMATTED */
      fprintf(outf, "\nx%sCL%s", presep, presep);
    int3printf(outf, corrupt_lines, sepchar, 0);
  }
  if (other_lines > 0) {
    if (aq == HTML)
      fprintf(outf, "\n<br><b>%s%s</b> ", lngstr[unwanted_], lngstr[colon_]);
    else if (aq == ASCII)
      fprintf(outf, "\n%s%s ", lngstr[unwanted_], lngstr[colon_]);
    else   /* aq == PREFORMATTED */
      fprintf(outf, "\nx%sUL%s", presep, presep);
    int3printf(outf, other_lines, sepchar, 0);
  }
  if (byq) {
    if (aq == HTML)
      fprintf(outf, "\n<br><b>%s%s</b> ", lngstr[totdata_], lngstr[colon_]);
    else if (aq == ASCII)
      fprintf(outf, "\n%s%s ", lngstr[totdata_], lngstr[colon_]);
    else   /* aq == PREFORMATTED */
      fprintf(outf, "\nx%sBT%s", presep, presep);
    bdivider = finddivider(total_bytes, bprefix);
    double3printf(outf, ROUND(total_bytes / bdivider), sepchar, 0);
    if (aq != PREFORMATTED)
      fprintf(outf, " %s%s", bprefix, lngstr[bytes_]);
    if (q7) {
      if (aq == PREFORMATTED)
	fprintf(outf, "\nx%sB7%s", presep, presep);
      else
	fprintf(outf, " (");
      bdivider = finddivider(total_bytes7, bprefix);
      double3printf(outf, ROUND(total_bytes7 / bdivider), sepchar, 0);
      if (aq != PREFORMATTED)
	fprintf(outf, " %s%s)", bprefix, lngstr[bytes_]);
    }
    if (totalmins > 30 && aq != PREFORMATTED) {
      if (aq == HTML)
	fprintf(outf, "\n<br><b>%s%s</b> ", lngstr[avedata_], lngstr[colon_]);
      else
	fprintf(outf, "\n%s%s ", lngstr[avedata_], lngstr[colon_]);
  
      bdivider = finddivider((total_bytes * 1440) / (totalmins + 0.0),
			     bprefix);
      double3printf(outf, ROUND((total_bytes * 1440) / (totalmins + 0.0) / bdivider), sepchar, 0);
      fprintf(outf, " %s%s", bprefix, lngstr[bytes_]);
      if (q7) {
	fprintf(outf, " (");
	bdivider = finddivider(total_bytes7 / 7.0, bprefix);
	double3printf(outf, ROUND(total_bytes7 / 7.0 / bdivider), sepchar, 0);
	fprintf(outf, " %s%s)", bprefix, lngstr[bytes_]);
      }
    }
  }
  if (q7 && aq != PREFORMATTED) {
    if (aq == HTML)
      fprintf(outf, "\n<br>");
    else
      fprintf(outf, "\n");
    fprintf(outf, "(%s ", lngstr[brackets_]);
      
    if (starttimec.code > totime.code) {
      fprintf(outf, "%s ", lngstr[sevendaysto_]);
      dateprintf(outf, lngstr[datefmt1_], totime.date, totime.monthno,
		 totime.year, UNSET, UNSET);
      fprintf(outf, ").");
    }
    else
      fprintf(outf, "%s).", lngstr[lastsevendays_]);
  }
  
  if (aq == HTML && (mq || Wq || dq || Dq || hq || oq || Sq || iq || rq))
    gotos(outf, 'z');
  else if (aq == ASCII) {
    fprintf(outf, "\n");
    asciiline(outf);
  }
  else if (aq == PREFORMATTED)
    fprintf(outf, "\n");
}
