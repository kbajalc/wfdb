/* file: refhr.c	G. Moody	20 March 1992
            Last revised:	  7 May 1999

-------------------------------------------------------------------------------
refhr: Sample program for generating heart rate measurement annotation file
Copyright (C) 1999 George B. Moody

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.

You may contact the author by e-mail (wfdb@physionet.org) or postal mail
(MIT Room E25-505A, Cambridge, MA 02139 USA).  For updates to this software,
please visit PhysioNet (http://www.physionet.org/).
_______________________________________________________________________________

This program copies an annotation file, inserting MEASURE annotations
containing two illustrative types of heart rate measurements into the output.
Any MEASURE annotations in the input file are not copied.  The output of this
program is suitable for input to `mxm'.
*/

// cc -lwfdb -Wl,-rpath,/usr/local/lib examples/refhr.c -o refhr

#include "wfdb/ecgcodes.h"
#include <stdio.h>
#include <wfdb/ecgmap.h>
#include <wfdb/wfdb.h>

int main(int argc, char *argv[]) {
    /* sampling frequency (samples per second) */
    double sps = 0.0;
    /* anntyp of previous beat label */
    static int a1;
    /* times of 3 previous beat labels */
    static long t1, t2, t3;
    /* current and previous 5 N-N intervals */
    static long nn0, nn1, nn2, nn3, nn4, nn5;

    static char *record;
    static WFDB_Anninfo ann[2];
    static WFDB_Annotation annot, hr_annot;

    /* temporary storage for heart rate measurements */
    char hr_string[20];

    /* Initialize the constant fields of the annotation structure which is
       to be used for heart rate measurements. */
    hr_annot.anntyp = MEASURE;
    hr_annot.aux = (void *)hr_string;

    int pass = 0;

    for (int i = 1; i < argc; i++) { /* read and interpret command arguments */
        if (*argv[i] == '-') {
            switch (*(argv[i] + 1)) {
            /* annotator names follow */
            case 'a':
                if (++i >= argc - 1) {
                    (void)fprintf(stderr, "%s: input and output annotator names must follow -a\n", argv[0]);
                    exit(1);
                }
                ann[0].name = argv[i];
                ann[0].stat = WFDB_READ;
                ann[1].name = argv[++i];
                ann[1].stat = WFDB_WRITE;
                break;

            /* record name follows */
            case 'r':
                if (++i >= argc) {
                    (void)fprintf(stderr, "%s: record name must follow -r\n", argv[0]);
                    exit(1);
                }
                record = argv[i];
                break;

            /* sampling freq */
            case 'z':
                if (++i >= argc) {
                    (void)fprintf(stderr, "%s: sampling frequency must follow -z\n", argv[0]);
                    exit(1);
                }
                sps = atof(argv[i]);
                if (sps <= 0.0) {
                    (void)fprintf(stderr, "%s: improper sampling frequency %s\n", argv[0], argv[i]);
                    exit(1);
                }
                break;

            /* copy qrs annotations */
            case 'c': pass = 1; break;

            default: (void)fprintf(stderr, "%s: unrecognized option %s\n", argv[0], argv[i]); exit(1);
            }
        } else {
            (void)fprintf(stderr, "%s: unrecognized argument %s\n", argv[0], argv[i]);
            exit(1);
        }
    }

    if (record == NULL || ann[0].name == NULL) {
        (void)fprintf(stderr, "usage: %s -r record -a input-annotator output-annotator [-z fs] [-c]\n", argv[0]);
        exit(1);
    }

    if (sps == 0.0 && (sps = sampfreq(record)) <= 0) {
        (void)fprintf(stderr, "%s: (warning) %g Hz sampling frequency assumed\n", argv[0], WFDB_DEFFREQ);
        sps = WFDB_DEFFREQ;
    }

    double tps = sampfreq(record);
    if (tps < 0) tps = sps;

    // (void)setsampfreq(sps);

    setafreq(sps);

    /* Open the input and output annotation files. */
    if (annopen(record, ann, 2) < 0) exit(2);

    // fprintf(stderr, "Input annotation file %s opened.  Sampling frequency = %g Hz\n", ann[0].name, tps);

    // setiafreq(0, sps);

    /* Read an annotation on each iteration. */
    while (getann(0, &annot) == 0) {
        WFDB_Time samp;
        if (tps == sps) {
            samp = annot.time;
        } else {
            samp = annot.time * sps / tps + 0.5;
            if (samp > annot.time * sps / tps + 0.5) samp--;
        }
        annot.time = samp;

        /* Copy the annotation to the output file, unless it's a MEASURE
           annotation (these get filtered out, so that they won't be
           mistaken for MEASURE annotations generated by this program) */
        if ((pass || annot.anntyp == RHYTHM) && annot.anntyp != MEASURE) (void)putann(0, &annot);

        /* Update heart rate measurements only if this annotation is a beat
           label. */
        if (!isqrs(annot.anntyp)) continue;

        /* This sample program calculates two simple heart rate
           measurements for illustrative purposes.  First is a
           measurement based on the last three R-R intervals, valid only
           after the fourth beat. */
        if (t3 > 0L) {
            double heart_rate = 180.0 * sps / (annot.time - t3);
            /* Convert the measurement to a string. */
            sprintf(hr_string + 1, "%g", heart_rate);
            /* The first byte of the `aux' field must be a byte count. */
            hr_string[0] = strlen(hr_string + 1);
            /* The measurement annotation can't be attached to the same
               sample as the current beat annotation, so attach it to
               the next sample. */
            hr_annot.time = annot.time + 1;
            /* The `subtyp' field specifies the measurement type.  Here
               the 3-beat average is assigned type 0. */
            hr_annot.subtyp = 0;
            /* Write the measurement annotation. */
            (void)putann(0, &hr_annot);
        }
        t3 = t2;
        t2 = t1;
        t1 = annot.time;

        /* The second measurement is based on the last six normal R-R
           intervals, valid only after six normal R-R intervals have
           been observed, and updated only if the current and previous
           beats are both normal. */
        if (map2(annot.anntyp) == NORMAL && a1 == NORMAL) {
            nn0 = t1 - t2;
            if (nn5 > 0L) {
                double heart_rate = 360.0 * sps / (nn5 + nn4 + nn3 + nn2 + nn1 + nn0);
                sprintf(hr_string + 1, "%g", heart_rate);
                hr_string[0] = strlen(hr_string + 1);
                /* This measurement annotation will need to be written
                   two sample intervals after the beat annotation, so
                   that it doesn't coincide with the first measurement. */
                hr_annot.time = annot.time + 2;
                /* The 6 N-N interval-based average is assigned type 1. */
                hr_annot.subtyp = 1;
                (void)putann(0, &hr_annot);
            }
            nn5 = nn4;
            nn4 = nn3;
            nn3 = nn2;
            nn2 = nn1;
            nn1 = nn0;
        }
        a1 = map2(annot.anntyp);

        /* Note: we might run into trouble if another input annotation
           follows a beat annotation within two sample intervals.  In this
           case, putann() will complain that annotations were not supplied
           in time order.  The measurement annotations will have been
           written properly, but the offending input annotation will not
           have been written.  This is harmless if we're only using the
           output file as input for `mxm', since `mxm' ignores everything
           but the measurement annotations anyway. */
    }

    wfdbquit(); /* close input files */
    exit(0);    /*NOTREACHED*/
}
