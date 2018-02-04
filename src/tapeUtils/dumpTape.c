/*
 Copyright 2013-2016 by Charles Anthony

 All rights reserved.

 This software is made available under the terms of the
 ICU License -- ICU 1.8.1 and later.
 See the LICENSE file at the top-level directory of this distribution and
 at https://sourceforge.net/p/dps8m/code/ci/master/tree/LICENSE
 */

/*
tape image format:

<32 bit little-endian blksiz> <data> <32bit little-endian blksiz>

a single 32 bit word of zero represents a file mark
*/

// From sim_tape.h
#define MTR_TMK         0x00000000                      /* tape mark */
#define MTR_EOM         0xFFFFFFFF                      /* end of medium */
#define MTR_GAP         0xFFFFFFFE                      /* primary gap */
#define MTR_RRGAP       0xFFFFFFFF                      /* reverse read half gap */
#define MTR_FHGAP       0xFFFEFFFF                      /* fwd half gap (overwrite) */
#define MTR_RHGAP       0xFFFF0000                      /* rev half gap (overwrite) */
#define MTR_M_RHGAP     (~0x000080FF)                   /* range mask for rev gap */
#define MTR_MAXLEN      0x00FFFFFF                      /* max len is 24b */
#define MTR_ERF         0x80000000                      /* error flag */
#define MTR_F(x)        ((x) & MTR_ERF)                 /* record error flg */
#define MTR_L(x)        ((x) & ~MTR_ERF)                /* record length */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

// extract bits into a number
#include <stdint.h>

#include "bit36.h"


int main (int argc, char * argv [])
  {
    int fd;
    uint32_t blksiz;
    int32_t blksiz2;
    //fd = open ("20184.tap", O_RDONLY);
    fd = open (argv [1], O_RDONLY);
    if (fd < 0)
      {
        printf ("can't open tape\n");
        exit (1);
      }
    int blkno = 0;
    for (;;)
      {
        ssize_t sz;
        sz = read (fd, & blksiz, sizeof (blksiz));
        if (sz == 0)
          break;
        if (sz != sizeof (blksiz))
          {
            printf ("can't read blksiz\n");
            exit (1);
          }
        printf ("blksiz %u\n", blksiz);

        //if (! blksiz)
        if (blksiz == MTR_TMK)
          {
            printf ("tapemark\n");
          }
        else if (blksiz == MTR_EOM)
          {
            printf ("end-of-medium\n");
            break;
          }
        else if (blksiz == MTR_GAP)
          {
            printf ("primary gap");
          }
        else if (blksiz == MTR_RRGAP)
          {
            printf ("reverse read half gap");
          }
        else if (blksiz == MTR_FHGAP)
          {
            printf ("fwd half gap");
          }
        else if (blksiz == MTR_RHGAP)
          {
            printf ("rev half gap");
          }
        else if (blksiz > MTR_MAXLEN)
          {
            printf ("unknown metadata type 0x%x\n", blksiz);
          }
        else
          {
// 72 bits at a time; 2 dps8m words == 9 bytes
            int i;
            uint8_t bytes [9];
            for (i = 0; i < blksiz; i += 9)
              {
                int n = 9;
                if (i + 9 > blksiz)
                  n = blksiz - i;
                memset (bytes, 0, 9);
                sz = read (fd, bytes, n);
//{ int jj; for (jj = 0; jj < 72; jj ++) { uint64_t k = extr (bytes, jj, 1); printf ("%ld", k); } printf ("\n"); }
//printf ("%02X %02X %02X %02X %02X\n", bytes [0], bytes [1], bytes [2], bytes [3], bytes [4]);
                if (sz == 0)
                  {
                    printf ("eof whilst skipping byte\n");
                    exit (1);
                  }
                if (sz != n)
                  {
                    printf ("can't skip bytes\n");
                    exit (1);
                  }
                uint64_t w1 = extr (bytes, 0, 36);
//printf ("%012"PRIo64"\n", w1);
                uint64_t w2 = extr (bytes, 36, 36);
                // int text [8]; // 8 9-bit bytes in 2 words
                printf ("%04d:%04o   %012"PRIo64"   %012"PRIo64"   \"", blkno, i * 2 / 9, w1, w2);
                int j;
//{printf ("\n<<%012"PRIo64">>\n", (* (uint64_t *) bytes) & 0777777777777); }

                static int byteorder [8] = { 3, 2, 1, 0, 7, 6, 5, 4 };
                for (j = 0; j < 8; j ++)
                  {
                    uint64_t c = extr (bytes, byteorder [j] * 9, 9);
                    if (isprint (c))
                      printf ("%c", (char) c);
                    else
                      printf ("\\%03lo", c);
                  }
                printf ("\n");
              }
            sz = read (fd, & blksiz2, sizeof (blksiz2));
            if (sz != sizeof (blksiz2))
              {
                printf ("can't read blksiz2\n");
                exit (1);
              }
            if (blksiz != blksiz2)
              {
                printf ("blksiz != blksiz2\n");
                exit (1);
              }
            blkno ++;
          }
      }
    return 0;
  }
