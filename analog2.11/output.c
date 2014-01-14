/*** analog 2.11 ***/
/* Please read Readme.html, or http://www.statslab.cam.ac.uk/~sret1/analog/  */

/*** output.c; the main output function. ***/
/* See output2.c for subsidiary functions */

#include "analhea2.h"

void output(struct genstruct *rsorthead, struct genstruct *isorthead,
	    struct genstruct *tsorthead, struct genstruct *Ssorthead,
	    int firstdom, struct genstruct *fsorthead,
	    struct genstruct *bsorthead, struct genstruct *Bsorthead,
	    int errorder[])
{
  extern char *outfile;
  extern char dayname[7][11];
  extern char monthname[12][12];
  extern char *lngstr[NOLNGSTRS];
  extern int monthlength[12];
  extern char *hostname;
  extern char *logourl;
  extern char *hosturl;
  extern char *commandname;
  extern char *headerfile;
  extern char *footerfile;
  extern char reportorder[];
  extern flag byq, refbyq, browbyq, html2;
  extern flag mback, Dback, Wback, Hback;
  extern int mrows, Hrows, Drows;
  extern flag xq, dq, Dq, Wq, hq, Hq, mq, Sq, rq, oq, iq, tq, fq, bq, Bq, cq;
  extern int eq;
  extern int aq, debug;
  extern struct include *linkhead, *reflinkhead;
  extern struct alias *routaliashead, *ioutaliashead, *Soutaliashead;
  extern struct alias *foutaliashead, *boutaliashead, *toutaliashead;
  extern struct timestruct firsttime, lasttime;
  extern time_t starttime, stoptime;
  extern int weekbeginson;
  extern struct monthly *firstm, *lastm;
  extern struct weekly *firstW, *lastW;
  extern struct daily *firstD, *lastD;
  extern struct hourly *firstH, *lastH;
  extern int dreq[], hreq[], dpag[], hpag[];
  extern double dbytes[], hbytes[];
  extern int munit, Wunit, hunit, Hunit, dunit, Dunit;
  extern double total_bytes, total_ref_bytes, total_brow_bytes;
  extern int total_succ_reqs, total_page_reqs, total_good_refs;
  extern int total_good_brows, total_ref_pages, total_brow_pages;
  extern int pagewidth;
  extern int Ssortby, rsortby, isortby, tsortby, fsortby, bsortby, Bsortby;
  extern char *Sminreqstr, *rminreqstr, *iminreqstr, *fminreqstr;
  extern char *tminreqstr, *bminreqstr, *Bminreqstr;
  extern char *Sminpagestr, *rminpagestr, *iminpagestr, *fminpagestr;
  extern char *tminpagestr, *bminpagestr, *Bminpagestr;
  extern char *Sminbytestr, *rminbytestr, *iminbytestr, *fminbytestr;
  extern char *tminbytestr, *bminbytestr, *Bminbytestr;
  extern char mgraph, dgraph, Dgraph, hgraph, Hgraph, Wgraph;
  extern char mcols[], dcols[], Dcols[], hcols[], Wcols[], Hcols[];
  extern char rcols[], icols[], tcols[], Scols[], fcols[], bcols[], Bcols[];
  extern char *imagedir, *baseurl;
  extern int imaxreqs, tmaxreqs, Smaxreqs, rmaxreqs, fmaxreqs, bmaxreqs;
  extern int Bmaxreqs;
  extern int imaxpages, tmaxpages, Smaxpages, rmaxpages, fmaxpages, bmaxpages;
  extern int Bmaxpages;
  extern double imaxbytes, tmaxbytes, Smaxbytes, rmaxbytes, fmaxbytes;
  extern double bmaxbytes, Bmaxbytes;
  extern char *presep;

  FILE *outf;       /* the output file */
  int fieldwidth;   /* Width we require to print #reqs in */
  int pfieldwidth, bfieldwidth;  /* #pages and #bytes ditto */
  char bprefix[2];  /* kilo, Mega, etc. */
  int graphwidth;   /* the width left for a graph after columns written */
  struct monthly *mp;
  struct daily *dp;
  struct weekly *wp;
  struct hourly *hp;
  int maxreq, maxpages;       /* within a particular date report */
  double maxbytes;
  double bdivider;
  int year, monthno, date, hr;
  flag finished;
  flag ispipe;
  int i, j, firsti, lasti, tempint;
  char *ro;
  char templine[MAXLINELENGTH];
  char tempstr[MAXSTRINGLENGTH];
  FILE *tempf;

  bprefix[0] = '\0';
  bprefix[1] = '\0';

  if (STREQ(outfile, "stdout") || STREQ(outfile, "-")) {
    outf = stdout;
    if (debug > 0)
      fprintf(stderr, "F: Opening stdout as output file\n");
  }

  else if ((outf = fopen(outfile, "a")) == NULL) {
    fprintf(stderr, "%s: Error: failed to open output file %s for writing.\n",
	    commandname, outfile);
    exit(ERR);  /* shouldn't get here because also tested at the beginning */
  }             /* (unless it's vanished in the meantime or something) */

  else if (debug > 0)
      fprintf(stderr, "F: Opening %s as output file\n", outfile);

  if (aq == CACHE)
    fprintf(outf,
	    "CACHE type 2 produced by analog%s. Do not modify or delete!",
	    VERSION);
  else {
    if (aq == HTML) {
      fprintf(outf, "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n");
      fprintf(outf, "<html>\n<head>\n");
      fprintf(outf, "<meta name=\"GENERATOR\" content=\"analog%s\">\n",
	      VERSION);
      fprintf(outf, "<title>%s ", lngstr[webstatsfor_]);
      htmlfprintf(outf, hostname);
      fprintf(outf, "</title></head>\n");
      fprintf(outf, "<body>\n<h1><a NAME=\"Top\">");
      if (!STREQ(logourl, "none")) {
	fprintf(outf, "<IMG src=\"");
	htmlfprintf(outf, logourl);
	fprintf(outf, "\" alt=\"\"> ");
      }
      if (hosturl[0] == '-') {
	fprintf(outf, "%s</a> ", lngstr[webstatsfor_]);
	htmlfprintf(outf, hostname);
      }
      else {
	fprintf(outf, "%s</a> <a HREF=\"", lngstr[webstatsfor_]);
	htmlfprintf(outf, hosturl);
	fprintf(outf, "\">");
	htmlfprintf(outf, hostname);
	fprintf(outf, "</a>");
      }
      fprintf(outf, "</h1>\n\n");
    }
    else if (aq == ASCII) {
      fprintf(outf, "%s %s\n", lngstr[webstatsfor_], hostname);
      matchlength(outf, hostname, '=');
      matchlength(outf, lngstr[webstatsfor_], '=');
      fprintf(outf, "=\n");
    }
    
    /* insert header file  -- before top line for HTML, ASCII; after for PRE */
    
    if (!STREQ(headerfile, "none")) {
      tempf = fopenlog(headerfile, "header file", &ispipe);
      if (tempf != NULL) {
	if (aq == HTML)
	  fprintf(outf, "<hr>\n");
	else if (aq == ASCII)
	  fprintf(outf, "\n");
	
	while(fgets(templine, MAXLINELENGTH, tempf) != NULL)
	  fprintf(outf, "%s", templine);
	if (templine[(int)strlen(templine) - 1] != '\n')
	  fprintf(outf, "\n");
	fcloselog(tempf, headerfile, "header file", ispipe);
	
	if (aq == ASCII) {
	  for (i = 0; i < pagewidth; i++)
	    fprintf(outf, "-");
	}
	
	if (aq != PREFORMATTED)
	  fprintf(outf, "\n");

	html2 = OFF;
      }
    }

    if (aq == PREFORMATTED) {
      if (xq) {
	fprintf(outf, "x%sHN%s%s", presep, presep, hostname);
	if (hosturl[0] != '-')
	  fprintf(outf, "\nx%sHU%s%s", presep, presep, hosturl);
      }
    }

  }   /* else (aq != CACHE) */

  /* Summary statistics */

  if (xq)
    gensum(outf);
  else if (aq == ASCII)
    printf("\n");

  /* Now for the rest of the reports, in reportorder order */
    
  for (ro = reportorder; *ro != '\0'; ro++) {

    switch(*ro) {

    case 'm':    /* Monthly report */

      if (mq) {

	/* For forwards reports and (mrows > 0), must get the first month
	   correct (for backwards reports doesn't matter; can just count off
	   the correct number of months as we go (using j below). */

	/* NB This could be more efficient by not remembering months that
	   we're definitely not going to need (re-using those years in the
	   date hashing). Then we only ever need to move forward by at most
	   one year. We couldn't use firsttime for the earliest month recorded
	   though. */

	/* Another alternative would be to have TO times for each report
	   separately. (This would allow them for the general reports too).
	   It wouldn't, however, extend well to the weekly report. */

	if (!mback && mrows > 0) {  /* set year, monthno to 1st month wanted */
	  monthno = lasttime.year * 12 + lasttime.monthno - mrows + 1;
	  year = monthno / 12;
	  monthno %= 12;
	  if (year < firsttime.year ||
	      (year == firsttime.year && monthno <= firsttime.monthno)) {
	    mrows = 0;
	    monthno = 0;
	  }
	  else for (i = year - firsttime.year; i > 0; i--)
	    firstm = firstm -> next;    /* run firstm through enough years */
	}
	else
	  monthno = 0;

	if (aq != PREFORMATTED) {
	  maxreq = 0;
	  maxpages = 0;
	  maxbytes = 0.0;
	  finished = FALSE;
	  for (mp = mback?lastm:firstm; !finished; mp = mp -> next) {
	    for (i = (mp == firstm)?monthno:0; i < 12; i++) {
	      maxreq = MAX(maxreq, mp -> reqs[i]);
	      maxpages = MAX(maxpages, mp -> pages[i]);
	      maxbytes = MAX(maxbytes, mp -> bytes[i]);
	    }
	    if (mp == (mback?firstm:lastm))
	      finished = TRUE;
	  }
	  datehead(outf, maxreq, maxpages, maxbytes, mcols, &mgraph, "Monthly",
		   lngstr[monthrep_], lngstr[month_],
		   (int)htmlstrlen(monthname[0]) + 5, 'm', &munit, &fieldwidth,
		   &pfieldwidth, &bfieldwidth, &graphwidth, &bdivider);
	}

	if (mback && mrows > 0)
	  j = mrows;
	else
	  j = -1;
	finished = FALSE;
	if (mback || mrows <= 0)     /* else 'year' already calculated */
	  year = (mback?(&lasttime):(&firsttime)) -> year;
	/* really (mback?lasttime:firsttime).year but some compilers choke */
	for (mp = mback?lastm:firstm; !finished; mp = mp -> next) {
	  if (mp == firstm) {
	    if (!mback && mrows > 0)
	      firsti = monthno;
	    else
	      firsti = firsttime.monthno;
	    if (mback)
	      finished = TRUE;
	  }
	  else
	    firsti = 0;
	  if (mp == lastm) {
	    lasti = lasttime.monthno;
	    if (!mback)
	      finished = TRUE;
	  }
	  else
	    lasti = 11;
	  for (i = mback?lasti:firsti; (mback?(i >= firsti):(i <= lasti)) &&
	       j != 0; i += mback?(-1):1) {  /* thro months in chosen order */
	    if (aq == PREFORMATTED)
	      precols(outf, mcols, 'm', byq, ON);
	    else
	      fprintf(outf, "%s %d: ", monthname[i], year);
	    dateline(outf, mp -> reqs[i], mp -> pages[i], mp -> bytes[i],
		     mcols, mgraph, fieldwidth, pfieldwidth, bfieldwidth,
		     munit, bdivider);
	    if (aq == PREFORMATTED)
	      fprintf(outf, "%d%s%d\n", year, presep, i + 1);
	    j--;
	  }
	  year += mback?(-1):1;
	}
	
	if (aq == ASCII)
	  asciiline(outf);
	else if (aq == HTML)
	  fprintf(outf, "</tt></pre>");
	
      }

      break;
      
    case 'W':      /* Weekly report */

      if (Wq) {

	/* For the weekly report, we needn't worry about Wrows at this stage.
	   Becuase there is only one week per structure, we never allocate
	   more than are needed. */

	if (aq != PREFORMATTED) {
	  maxreq = 0;
	  maxpages = 0;
	  maxbytes = 0.0;
	  finished = FALSE;
	  for (wp = Wback?lastW:firstW; !finished; wp = wp -> next) {
	    maxreq = MAX(maxreq, wp -> reqs);
	    maxpages = MAX(maxpages, wp -> pages);
	    maxbytes = MAX(maxbytes, wp -> bytes);
	    if (wp == (Wback?firstW:lastW))
	      finished = TRUE;
	  }
	  datehead(outf, maxreq, maxpages, maxbytes, Wcols, &Wgraph, "Weekly",
		   lngstr[weekrep_], lngstr[weekbeg_],
		   (int)datefmtlen(lngstr[datefmt0_]), 'W', &Wunit,
		   &fieldwidth, &pfieldwidth, &bfieldwidth, &graphwidth,
		   &bdivider);
	}

	finished = FALSE;
	for (wp = Wback?lastW:firstW; !finished; wp = wp -> next) {
	  if (aq == PREFORMATTED)
	    precols(outf, Wcols, 'W', byq, ON);
	  else {
	    dateprintf(outf, lngstr[datefmt0_], wp -> start.date,
		       wp -> start.monthno, wp -> start.year, UNSET, UNSET);
	    fprintf(outf, ": ");
	  }
	  dateline(outf, wp -> reqs, wp -> pages, wp -> bytes, Wcols, Wgraph,
		   fieldwidth, pfieldwidth, bfieldwidth, Wunit, bdivider);
	  if (aq == PREFORMATTED)
	    fprintf(outf, "%d%s%d%s%d\n", wp -> start.year, presep,
		    wp -> start.monthno + 1, presep, wp -> start.date);
	  if (wp == (Wback?firstW:lastW))
	    finished = TRUE;
	  
	}     /* end running through weeks */

	if (aq == ASCII)
	  asciiline(outf);
	else if (aq == HTML)
	  fprintf(outf, "</tt></pre>");
	
      }   /* end if Wq */

      break;

    case 'd':      /* Daily summary */

      if (dq) {
	
	if (aq != PREFORMATTED) {
	  maxreq = 0;
	  maxpages = 0;
	  maxbytes = 0.0;
	  for (i = 0; i <= 6; i++) {
	    maxreq = MAX(maxreq, dreq[i]);
	    maxpages = MAX(maxpages, dpag[i]);
	    maxbytes = MAX(maxbytes, dbytes[i]);
	  }
	  datehead(outf, maxreq, maxpages, maxbytes, dcols, &dgraph, "Daily",
		   lngstr[daysum_], lngstr[day_], (int)htmlstrlen(dayname[0]),
		   'd', &dunit, &fieldwidth, &pfieldwidth, &bfieldwidth,
		   &graphwidth, &bdivider);
	}

	for(i = 0; i <= 6; i++) {
	  j = (weekbeginson + i) % 7;
	  if (aq == PREFORMATTED)
	    precols(outf, dcols, 'd', byq, ON);
	  else
	    fprintf(outf, "%s: ", dayname[j]);
	  dateline(outf, dreq[j], dpag[j], dbytes[j], dcols, dgraph,
		   fieldwidth, pfieldwidth, bfieldwidth, dunit, bdivider);
	  if (aq == PREFORMATTED)
	    fprintf(outf, "%s\n", dayname[j]);
	}
	
	if (aq == ASCII)
	  asciiline(outf);
	else if (aq == HTML)
	  fprintf(outf, "</tt></pre>");
	
      }

      break;
      
    case 'D':       /* Full daily report */

      if (Dq) {

	/* For the daily report, we adopt a slightly different strategy to that
	   for the monthly report above. Rather than calculate the first day
	   explicitly, we move along the months until there are only the right
	   number of days left. */

	if (!Dback && Drows > 0) {
	  tempint = minsbetween(1, firsttime.monthno, firsttime.year, 0, 0,
             lasttime.date, lasttime.monthno, lasttime.year, 0, 0) / 1440 + 1;
	  /* = days between start of first month, and last date, inc. */
	  monthno = firsttime.monthno;
	  year = firsttime.year;
	  if (tempint - firsttime.date < Drows) {
	    Drows = 0;
	    date = 1;
	  }
	  else {
	    while ((tempint -= monthlength[monthno] +
		    ISLEAPFEB(monthno, year)) > Drows) {
	      firstD = firstD -> next;
	      if (++monthno == 12) {
		monthno = 0;
		year++;
	      }
	    }      
	    date = tempint + monthlength[monthno] + ISLEAPFEB(monthno, year) -
	      Drows + 1;  /* when at right month, find initial date wanted */
	  }
	}
	else
	  date = 1;

	if (aq != PREFORMATTED) {
	  maxreq = 0;
	  maxpages = 0;
	  maxbytes = 0.0;
	  finished = FALSE;
	  for (dp = Dback?lastD:firstD; !finished; dp = dp -> next) {
	    for (i = (dp == firstD)?(date - 1):0; i < 31; i++) {
	      maxreq = MAX(maxreq, dp -> reqs[i]);
	      maxpages = MAX(maxpages, dp -> pages[i]);
	      maxbytes = MAX(maxbytes, dp -> bytes[i]);
	    }
	    if (dp == (Dback?firstD:lastD))
	      finished = TRUE;
	  }
	  datehead(outf, maxreq, maxpages, maxbytes, Dcols, &Dgraph,
		   "FullDaily", lngstr[dayrep_], lngstr[date_],
		   (int)datefmtlen(lngstr[datefmt0_]), 'D', &Dunit,
		   &fieldwidth, &pfieldwidth, &bfieldwidth, &graphwidth,
		   &bdivider);
	}
	
	if (Dback && Drows > 0)
	  j = Drows;
	else
	  j = -1;
	finished = FALSE;
	if (Dback || Drows <= 0) {  /* o/wise year and monthno already found */
	  year = (Dback?(&lasttime):(&firsttime)) -> year;
	  monthno = (Dback?(&lasttime):(&firsttime)) -> monthno;
	}
	for (dp = Dback?lastD:firstD; !finished; dp = dp -> next) {
	  if (dp == firstD) {
	    if (!Dback && Drows > 0)
	      firsti = date - 1;
	    else
	      firsti = firsttime.date - 1;
	    if (Dback)
	      finished = TRUE;
	  }
	  else
	    firsti = 0;
	  if (dp == lastD) {
	    lasti = lasttime.date - 1;
	    if (!Dback)
	      finished = TRUE;
	  }
	  else
	    lasti = monthlength[monthno] + ISLEAPFEB(monthno, year) - 1;
	  for (i = Dback?lasti:firsti; (Dback?(i >= firsti):(i <= lasti)) &&
	       j != 0; i += Dback?(-1):1) { /* thro days in chosen order */
	    if (aq == PREFORMATTED)
	      precols(outf, Dcols, 'D', byq, ON);
	    else {
	      dateprintf(outf, lngstr[datefmt0_], i + 1, monthno, year,
			 UNSET, UNSET);
	      fprintf(outf, ": ");
	    }
	    dateline(outf, dp -> reqs[i], dp -> pages[i], dp -> bytes[i],
		     Dcols, Dgraph, fieldwidth, pfieldwidth, bfieldwidth,
		     Dunit, bdivider);
	    if (aq == PREFORMATTED)
	      fprintf(outf, "%d%s%d%s%d\n", year, presep, monthno + 1, presep,
		      i + 1);
	    else if (((dayofdate(i + 1, monthno, year) + (!Dback)) % 7 ==
		    weekbeginson) && !(finished && i == (Dback?firsti:lasti)))
	      fprintf(outf, "\n");
                         /* extra blank line after each week (not last) */
	    j--;
	  }

	  if (Dback) {
	    if ((--monthno) == -1) {
	      monthno = 11;
	      --year;
	    }
	  }
	  else {
	    if ((++monthno) == 12) {
	      monthno = 0;
	      ++year;
	    }
	  }
	  
	}     /* end running through dp's */

	if (aq == ASCII)
	  asciiline(outf);
	else if (aq == HTML)
	  fprintf(outf, "</tt></pre>");
	
      }   /* end if Dq */

      break;

    case 'H':       /* Full hourly report */

      /* This is essentially like the daily report above */

      if (Hq) {

	if (!Hback && Hrows > 0) {
	  tempint = minsbetween(firsttime.date, firsttime.monthno,
	      firsttime.year, 0, 0, lasttime.date, lasttime.monthno,
	      lasttime.year, lasttime.hr, 0) / 60 + 1;
	  /* = hrs between start of first day, and last hr, inc. */
	  date = firsttime.date;
	  monthno = firsttime.monthno;
	  year = firsttime.year;
	  if (tempint - firsttime.hr < Hrows) {
	    Hrows = 0;
	    hr = 0;
	  }
	  else {
	    for ( ; tempint - Hrows > 23; tempint -= 24) {
	      firstH = firstH -> next;
	      if (++date > monthlength[monthno] + ISLEAPFEB(monthno, year)) {
		date = 1;
		if (++monthno == 12) {
		  monthno = 0;
		  year++;
		}
	      }
	    }
	    hr = tempint - Hrows;
	  }
	}
	else
	  hr = 0;

	if (aq != PREFORMATTED) {
	  maxreq = 0;
	  maxpages = 0;
	  maxbytes = 0.0;
	  finished = FALSE;
	  for (hp = Hback?lastH:firstH; !finished; hp = hp -> next) {
	    for (i = ((hp == firstH)?hr:0); i < 24; i++) {
	      maxreq = MAX(maxreq, hp -> reqs[i]);
	      maxpages = MAX(maxpages, hp -> pages[i]);
	      maxbytes = MAX(maxbytes, hp -> bytes[i]);
	    }
	    if (hp == (Hback?firstH:lastH))
	      finished = TRUE;
	  }
	  sprintf(tempstr, "%s:%s", lngstr[date_], lngstr[hr_]);
	  if (aq != CACHE)
	    datehead(outf, maxreq, maxpages, maxbytes, Hcols, &Hgraph,
		     "FullHourly", lngstr[hourrep_], tempstr,
		     (int)datefmtlen(lngstr[datefmt0_]) + 3, 'H', &Hunit,
		     &fieldwidth, &pfieldwidth, &bfieldwidth, &graphwidth,
		     &bdivider);
	}

	if (Hback && Hrows > 0)
	  j = Hrows;
	else
	  j = -1;
	finished = FALSE;
	if (Hback || Hrows <= 0) {
	  year = (Hback?(&lasttime):(&firsttime)) -> year;
	  monthno = (Hback?(&lasttime):(&firsttime)) -> monthno;
	  date = (Hback?(&lasttime):(&firsttime)) -> date;
	}
	for (hp = Hback?lastH:firstH; !finished; hp = hp -> next) {
	  if (hp == firstH) {
	    if (!Hback && Hrows > 0)
	      firsti = hr;
	    else
	      firsti = firsttime.hr;
	    if (Hback)
	      finished = TRUE;
	  }
	  else
	    firsti = 0;
	  if (hp == lastH) {
	    lasti = lasttime.hr;
	    if (!Hback)
	      finished = TRUE;
	  }
	  else
	    lasti = 23;
	  for (i = Hback?lasti:firsti; (Hback?(i >= firsti):(i <= lasti)) &&
	       j != 0; i += Hback?(-1):1) {  /* through hrs in chosen order */
	    if (aq == CACHE) {
	      if (i == 0 || (hp == firstH && i == firsti))
		fprintf(outf, "\n%d%02d%02d%02d", year, monthno + 1, date, i);
	      fprintf(outf, ":%d:%d:%.0f", hp -> reqs[i], hp -> pages[i],
		      hp -> bytes[i]);
	    }
	    else {
	      if (aq == PREFORMATTED)
		precols(outf, Hcols, 'H', byq, ON);
	      else {
		dateprintf(outf, lngstr[datefmt0_], date, monthno, year,
			   UNSET, UNSET);
		fprintf(outf, ":%02d: ", i);
	      }
	      dateline(outf, hp -> reqs[i], hp -> pages[i], hp -> bytes[i],
		       Hcols, Hgraph, fieldwidth, pfieldwidth, bfieldwidth,
		       Hunit, bdivider);
	      if (aq == PREFORMATTED)
		fprintf(outf, "%d%s%d%s%d%s%d\n", year, presep, monthno + 1,
			presep, date, presep, i);
	      else if (i == (Hback?0:23) && !finished)
		fprintf(outf, "\n");
	      /* extra blank line after each day (not last) */
	      j--;
	    }
	  }

	  if (Hback) {
	    if ((--date) == 0) {
	      if ((--monthno) == -1) {
		monthno = 11;
		--year;
	      }
	      date = monthlength[monthno] + ISLEAPFEB(monthno, year);
	    }
	  }
	  else {
	    if ((++date) > monthlength[monthno] + ISLEAPFEB(monthno, year)) {
	      if ((++monthno) == 12) {
		monthno = 0;
		++year;
	      }
	      date = 1;
	    }
	  }
	  
	}     /* end running through hp's */

	if (aq == CACHE)
	  fprintf(outf, ":*\n");
	else if (aq == ASCII)
	  asciiline(outf);
	else if (aq == HTML)
	  fprintf(outf, "</tt></pre>");
	
      }   /* end if Hq */
    
      break;

    case 'h': /* Hourly summary */

      if (hq) {

	if (aq != PREFORMATTED) {
	  maxreq = 0;
	  maxpages = 0;
	  maxbytes = 0.0;
	  for (i = 0; i <= 23; i++) {
	    maxreq = MAX(maxreq, hreq[i]);
	    maxpages = MAX(maxpages, hpag[i]);
	    maxbytes = MAX(maxbytes, hbytes[i]);
	  }
	  datehead(outf, maxreq, maxpages, maxbytes, hcols, &hgraph, "Hourly",
		   lngstr[hoursum_], lngstr[hr_], 2, 'h', &hunit, &fieldwidth,
		   &pfieldwidth, &bfieldwidth, &graphwidth, &bdivider);
	}

	for(i = 0; i <= 23; i++) {
	  if (aq == PREFORMATTED)
	    precols(outf, hcols, 'h', byq, ON);
	  else
	    fprintf(outf, "%2d: ", i);
	  dateline(outf, hreq[i], hpag[i], hbytes[i], hcols, hgraph,
		   fieldwidth, pfieldwidth, bfieldwidth, hunit, bdivider);
	  if (aq == PREFORMATTED)
	    fprintf(outf, "%d\n", i);
	}
	
	if (aq == ASCII)
	  asciiline(outf);
	else if (aq == HTML)
	  fprintf(outf, "</tt></pre>");
	
      }
      
      break;

    case 'o':    /* Domain report */

      if (oq)
	domout(outf, firstdom);
	
      break;

    case 'S':    /* Host report */

      if (Sq) {
 	genout(outf, Ssorthead, total_succ_reqs, total_page_reqs, total_bytes,
	       Ssortby, Sminreqstr, Sminpagestr, Sminbytestr, Smaxreqs,
	       Smaxpages, Smaxbytes, Scols, "Host", lngstr[hostrep_],
	       lngstr[hostgs_], lngstr[hostgp_], lngstr[host_],
	       lngstr[hostgen_][0], 'S', Ssortby == ALPHABETICAL,
	       byq, ON, NULL, Soutaliashead, "");
      }
      break;

    case 'i':   /* Directory report */
   
      if (iq) {
	genout(outf, isorthead, total_succ_reqs, total_page_reqs, total_bytes,
	       isortby, iminreqstr, iminpagestr, iminbytestr, imaxreqs,
	       imaxpages, imaxbytes, icols, "Directory", lngstr[dirrep_],
	       lngstr[dirgs_], lngstr[dirgp_], lngstr[dir_],
	       lngstr[dirgen_][0], 'i', FALSE, byq, ON, NULL,
	       ioutaliashead, "");
      }
      break;

    case 't':   /* File type report */
   
      if (tq) {
	genout(outf, tsorthead, total_succ_reqs, total_page_reqs,
	       total_bytes, tsortby, tminreqstr, tminpagestr, tminbytestr,
	       tmaxreqs, tmaxpages, tmaxbytes, tcols, "FileType",
	       lngstr[typerep_], lngstr[extgs_], lngstr[extgp_], lngstr[ext_],
	       lngstr[extgen_][0], 't', FALSE, byq, ON, NULL,
	       toutaliashead, "");
      }
      break;

    case 'r':    /* Request report */
      
      if (rq) {
	genout(outf, rsorthead, total_succ_reqs, total_page_reqs, total_bytes,
	       rsortby, rminreqstr, rminpagestr, rminbytestr, rmaxreqs,
	       rmaxpages, rmaxbytes, rcols, "Request", lngstr[reqrep_],
	       lngstr[filegs_], lngstr[filegp_], lngstr[file_],
	       lngstr[filegen_][0], 'r', FALSE, byq, ON,
	       (aq == HTML)?linkhead:NULL, routaliashead, baseurl);
      }
      break;

    case 'f':    /* Referrer report */

      if (fq) {
	genout(outf, fsorthead, total_good_refs, total_ref_pages,
	       total_ref_bytes, fsortby, fminreqstr, fminpagestr, fminbytestr,
	       fmaxreqs, fmaxpages, fmaxbytes, fcols, "Referrer",
	       lngstr[refrep_], lngstr[refgs_], lngstr[refgp_], lngstr[url_],
	       lngstr[refgen_][0], 'f', FALSE, refbyq,
	       ON, (aq == HTML)?reflinkhead:NULL, foutaliashead, "");
      }
      break;

    case 'b':    /* Browser summary */

      if (bq) {
	genout(outf, bsorthead, total_good_brows, total_brow_pages,
	       total_brow_bytes, bsortby, bminreqstr, bminpagestr,
	       bminbytestr, bmaxreqs, bmaxpages, bmaxbytes, bcols, "Browser",
	       lngstr[browsum_], lngstr[browgs_], lngstr[browgp_],
	       lngstr[browser_], lngstr[browgen_][0], 'b', FALSE,
	       browbyq, browbyq, NULL, boutaliashead, "");
      }
      break;
      
    case 'B':    /* Full browser report */

      if (Bq) {
	genout(outf, Bsorthead, total_good_brows, total_brow_pages,
	       total_brow_bytes, Bsortby, Bminreqstr, Bminpagestr,
	       Bminbytestr, Bmaxreqs, Bmaxpages, Bmaxbytes, Bcols,
	       "FullBrowser", lngstr[browrep_], lngstr[browgs_],
	       lngstr[browgp_], lngstr[browser_], lngstr[browgen_][0], 'B',
	       FALSE, browbyq, browbyq, NULL, NULL, "");
      }
      break;

    case 'c':

      if (cq)
	statusout(outf);
      break;

    case 'e':

      if (eq)
	errout(outf, errorder);
      break;

    }    /* end switch */
  }      /* end for ro */

  /*** Bit at the bottom of the page ***/

  if (aq != CACHE) {
    if (aq != PREFORMATTED) {
      if (!aq)
	  fprintf(outf, "\n\n<hr>\n<i>%s <a HREF=\"http://www.statslab.cam.ac.uk/~sret1/analog/\">analog%s</a>.\n", lngstr[credit_], VERSION);
      else
	fprintf(outf, "%s analog%s.\n", lngstr[credit_], VERSION);

      time(&stoptime);
  
      stoptime -= starttime;   /* so now measures elapsed time */

      if (stoptime == 0) {
	if (aq == HTML)
	  fprintf(outf, "<br><b>%s%s</b> %s %s.</i>\n", lngstr[runtime_],
		  lngstr[colon_], lngstr[lessone_], lngstr[second_]);
	else
	  fprintf(outf, "%s%s %s %s.\n", lngstr[runtime_], lngstr[colon_],
		  lngstr[lessone_], lngstr[second_]);
      }

      else if (stoptime < 60) {
	if (aq == HTML)
	  fprintf(outf, "<br><b>%s%s</b> %d %s.</i>\n",
		  lngstr[runtime_], lngstr[colon_], stoptime,
		  (stoptime == 1)?lngstr[second_]:lngstr[seconds_]);
	else
	  fprintf(outf, "%s%s %d %s.\n", lngstr[runtime_], lngstr[colon_], 
		  stoptime, (stoptime == 1)?lngstr[second_]:lngstr[seconds_]);
      }
  
      else {
	if (aq == HTML)
	  fprintf(outf, "<br><b>%s%s</b> %d %s, %d %s.</i>\n",
		  lngstr[runtime_], lngstr[colon_], stoptime / 60,
		  (stoptime / 60 == 1)?lngstr[minute_]:lngstr[minutes_],
		  stoptime % 60,
		  (stoptime % 60 == 1)?lngstr[second_]:lngstr[seconds_]);
	else
	  fprintf(outf, "%s%s %d %s, %d %s.\n",
		  lngstr[runtime_], lngstr[colon_], stoptime / 60,
		  (stoptime / 60 == 1)?lngstr[minute_]:lngstr[minutes_],
		  stoptime % 60,
		  (stoptime % 60 == 1)?lngstr[second_]:lngstr[seconds_]);
      }
  
      if (!aq && (mq || Wq || dq || Dq || hq || oq || Sq || iq || rq)) {
	gotos(outf, '\0');
      }
    }     /* end if aq != PREFORMATTED */

    /* Finally, insert footer file */

    if (!STREQ(footerfile, "none")) {
      tempf = fopenlog(footerfile, "footer file", &ispipe);
      if (tempf != NULL) {
	if (aq == HTML)
	  fprintf(outf, "<hr>\n");
	else if (aq == ASCII) {
	  for (i = 0; i < pagewidth; i++)
	    fprintf(outf, "-");
	  fprintf(outf, "\n\n");
	}
	fflush(stdout);

	while(fgets(templine, MAXLINELENGTH, tempf) != NULL)
	  fprintf(outf, "%s", templine);
	if (templine[(int)strlen(templine) - 1] != '\n')
	  fprintf(outf, "\n");
	fcloselog(tempf, footerfile, "footer file", ispipe);

	if (aq == HTML || aq == ASCII)
	  fprintf(outf, "\n");

	html2 = OFF;
      }
    }

    if (aq == HTML) {
      if (html2) {
	fprintf(outf,
		"<P> <A HREF=\"http://www.webtechs.com/html-val-svc/\">\n");
	fprintf(outf, "<IMG SRC=\"");
	htmlfprintf(outf, imagedir);
	fprintf(outf, "html2.gif\"\n");
	fprintf(outf, "ALT=\"HTML 2.0 Conformant!\"></A>\n");
      }
      fprintf(outf, "\n</body>\n</html>\n");
    }
  }      /* end if aq != CACHE */

  fclose(outf);
  if (debug > 0)
    fprintf(stderr, "F: Closing %s\n", STREQ(outfile, "-")?"stdout":outfile);

}
