/*
 Copyright 2013-2016 by Charles Anthony

 All rights reserved.

 This software is made available under the terms of the
 ICU License -- ICU 1.8.1 and later.
 See the LICENSE file at the top-level directory of this distribution and
 at https://sourceforge.net/p/dps8m/code/ci/master/tree/LICENSE
 */

// The web.mit.edu ".9" files are packed 9 bits of data to a little endian
// 16 bit word.

// Try appending ".9"; if that exists, use it; else use the asciified.

// p9archive_bind_to_ascii p9File asciiDir
// Only extract the .bind components, if any

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>


// extract bits into a number
#include <stdint.h>

#include "bit36.h"

static void trimTrailingSpaces (char * str)
  {
    char * end = str + strlen(str) - 1;
    while (end > str && isspace (* end))
      end --;
    * (end + 1) = 0;
  }

static void makeDirs (char * name)
  {
    //printf (">%s\n", name);
    char * outname = strdup (name);
    char * outdir = dirname (outname);
    if (strlen (outdir) && strcmp (outdir, "."))
      {
        makeDirs (outdir);
        mkdir (outdir, 0777);
      }
    free (outname);
  }

int main (int argc, char * argv [])
  {
    if (argc != 3)
      {
        fprintf (stderr, "Usage: p72archive_bind_to_ascii archive directory\n");
        exit (1);
      }
    int fdin;

    uint8_t * big = NULL;
    ssize_t nchars;

// Try name.9

    char name9 [strlen (argv[1] + 3)];
    strcpy (name9, argv[1]);
    strcat (name9, ".9");

    fdin = open (name9, O_RDONLY);
    if (fdin >= 0)
      {
        off_t flen = lseek (fdin, 0, SEEK_END);
        lseek (fdin, 0, SEEK_SET);

        nchars = (flen + 1) / 2;
        big = malloc (nchars);
        uint8_t * bigp = big;

        for (;;)
          {
            ssize_t sz;
            uint16_t byte = 0;
            sz = read (fdin, & byte, 2);
            if (sz == 0)
              break;
            * (bigp ++) = (byte>>8) & 0377;
          }
      } // if (fdin >= 0)
    else
      {
// Try without the .9; the ascified version
        fdin = open (argv [1], O_RDONLY);
        if (fdin >= 0)
          {
            off_t flen = lseek (fdin, 0, SEEK_END);
            lseek (fdin, 0, SEEK_SET);

            nchars = flen;
            big = malloc (nchars);
            read (fdin, big, nchars);
          }
        else
          {
            fprintf (stderr, "can't open input file '%s'\n", argv[1]);
            exit (1);
          }
      }

    char dname [strlen (argv [2]) + 8];
    strcpy (dname, argv [2]);
    strcat (dname, "/./");
    //makeDirs (dname);

    for (uint i = 0; i < nchars; /* i ++ */)
      {
        // printf ("top %u %u\n", i, i %4u);
        if (i % 4u)
          {
            fprintf (stderr, "phase error\n");
            exit (1);
          }
        static uint8_t hdr [8] = "\014\012\012\012\017\012\011\011";
        if (strncmp ((char *) big + i, (char *) hdr, 8))
          {
for (uint j = 0; j < 8; j ++) fprintf (stderr, "%03o\n", big[i]);
            fprintf (stderr, "missing hdr %s'\n", argv[1]);
            exit (1);
          }
        //uint hdrStart = i;

        char segname [32 + 1];
        strncpy (segname, (char *) (big + i + 12u), 32);
        segname [32] = '\0';
        trimTrailingSpaces (segname);
        //printf ("%s\n", segname);
        for (i += 014u; i < nchars; i += 4u)
          {
            //printf ("try %d (%u)\n", i, i);
            static uint8_t trlr [8] = "\017\017\017\017\012\012\012\012";
            if (strncmp ((char *) big + i, (char *) trlr, 8) == 0)
              break;
          }
        if (i >= nchars)
          {
            fprintf (stderr, "missing trlr\n");
            exit (1);
          }
        i += 8u; // skip trailer
        //printf ("hdr len %u (%o)\n", i - hdrStart, i - hdrStart);

        // find end
        uint j;
        for (j = i; j < nchars; j += 4)
          {
//printf ("j %u\n", j);
            if (strncmp ((char *) big + j, (char *) hdr, 8) == 0)
              break;
          }
        // j now points to the next hdr or just past the buffer end;
        //printf ("component %d - %d (%o - %o)\n", i, j - 1, (i / 4), ((j - 1) / 4));
        uint last = j - 1;
        while (big [last] == 0)
          last --;
        //printf ("trimmed %u\n", j - last);
        if (j - last > 7)
          fprintf (stderr, "trimmed %u\n", j - last);

        if (strstr (segname, ".bind"))
          {
            makeDirs (dname);
            char fname [strlen (dname) + 32 + 3];
            strcpy (fname, dname);
            strcat (fname, segname);

            int fdout;
            fdout = creat (fname, 0777);
            if (fdout < 0)
              {
                fprintf (stderr, "can't open output file <%s>\n", fname);
                exit (1);
              }
            write (fdout, big + i, (last - i) + 1u);
            close (fdout);
          }
        i = j;
        // printf ("i now %u %u\n", i, i % 4u);
      }        
    return 0;
  }

