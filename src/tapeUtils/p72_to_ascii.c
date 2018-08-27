/*
 Copyright 2013-2016 by Charles Anthony

 All rights reserved.

 This software is made available under the terms of the
 ICU License -- ICU 1.8.1 and later.
 See the LICENSE file at the top-level directory of this distribution and
 at https://sourceforge.net/p/dps8m/code/ci/master/tree/LICENSE
 */

// p72_to_ascii p72File asciiFile

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

static void makeDirs (char * name)
  {
    //printf (">%s\n", name);
    if (strcmp (name, "/") == 0)
      return;
    char * outname = strdup (name);
    char * outdir = dirname (outname);
    if (strlen (outdir) && strcmp (outdir, "."))
      {
        makeDirs (outdir);
        mkdir (outdir, 0777);
      }
    free (outname);
  }

// Read the metadata file for a .p72 file, if present.
static void read_md (const char * name, long int * bitcnt)
  {
    * bitcnt = -1;

    // Given "/foo/bar", produce "/foo/.bar.md".
    char * mdname = calloc (strlen (name) + 5, 1);
    strcpy (mdname, name);
    char * start = strrchr (mdname, '/');
    if (start)
      {
        ++start;
      }
    else
      {
        start = mdname;
      }
    memmove (start + 1, start, strlen (start) + 1);
    *start = '.';
    strcat (start, ".md");

    FILE *f = fopen (mdname, "r");
    free (mdname);

    if (f)
      {
        fscanf (f, "bitcnt: %ld\n", bitcnt);
        fclose (f);
      }
  }

int main (int argc, char * argv [])
  {
    if (argc != 3)
      {
        fprintf (stderr, "Usage: p72_to_ascii p72File asciiFile\n");
        exit (1);
      }

    int fdin;
    fdin = open (argv [1], O_RDONLY);
    if (fdin < 0)
      {
        fprintf (stderr, "can't open input file <%s>\n", argv [1]);
        exit (1);
      }

    struct stat st;
    if (fstat (fdin, &st) < 0)
      {
        fprintf (stderr, "fstat failed on <%s>\n", argv [1]);
        exit (1);
      }
    // We're reading 8 bit input and repacking into 9 bit output.
    long int nchars = (st.st_size * 8L) / 9;

    long int bitcnt;
    read_md (argv [1], & bitcnt);
    if (bitcnt != -1)
      {
        if ((bitcnt % 9) != 0)
          {
            fprintf (stderr, "md bitcount %ld is not a multiple of 9 for <%s>\n", bitcnt, argv [1]);
            exit (1);
          }

        // There should be at most 72 - 9 = 63 bits = 7 9-bit chars of padding at the end.
        long int bcchars = bitcnt / 9;
        if (bcchars < (nchars - 7) || bcchars > nchars)
          {
            fprintf (stderr, "md char count %ld is inconsistent with input char count %ld for <%s>\n",
                     bcchars, nchars, argv [1]);
            exit (1);
          }

        nchars = bcchars;
      }

    makeDirs (argv [2]);
    int fdout;
    fdout = creat (argv [2], 0777);
    if (fdout < 0)
      {
        fprintf (stderr, "can't open output file <%s>\n", argv [2]);
        exit (1);
      }
    
    while (nchars > 0)
      {
        ssize_t sz;
// 72 bits at a time; 2 dps8m words == 9 bytes
        uint8_t bytes [9];
        memset (bytes, 0, 9);
        sz = read (fdin, bytes, 9);
        if (sz == 0)
          break;
// bytes bits  bits/ 9
//   1     8     0
//   2    16     1
//   3    24     2
//   4    32     3
//   5    40     4
//   6    48     5
//   7    56     6
//   8    64     7
//   9    72     8

        int nch = sz - 1;

        int j;

        static int byteorder [8] = { 3, 2, 1, 0, 7, 6, 5, 4 };
        char buf [8];
        for (j = 0; j < nch && nchars > 0; j ++)
          {
            uint64_t c = extr (bytes, byteorder [j] * 9, 9);
            //if (isprint (c))
              //printf ("%c", (char) c);
            //else
              //printf ("\\%03lo", c);
            buf [j] = c & 0177;
            write (fdout, & buf [j], 1);
            nchars--;
          }
        //printf ("\n");
        //write (fdout, buf, nch);
      }
    close (fdin);
    close (fdout);
    return 0;
  }

