/*

1. Install WFDB https://www.physionet.org/content/wfdb/10.7.0/
2. Compile according to your OS, e.g.: gcc -lwfdb -o nstcmp[.exe] nstcmp.c
3. Download the database from PhysioNet, e.g. MIT-BIH Arrhythmia Database
4. Unpack the downloaded database files
5. In the same directory put your noise annotation files, e.g. extension ".nst"
6. Run the nstcmp for each reacord:
   nstcmp -r <record> -a atr nst -L -N <output> -f 0 -t <length> [-c <channel>]

Options:
   Replace -t <length> with the actual record length, format: mins:secs
   Replace -c <channel> with the actual channel number you want to analyze, default is 0
   nstcmp -r 105 -a atr nst -L -N nstcmp.out -f 0 -t 30:0 -c 0

Format of the annotation files is given at:

https://archive.physionet.org/physiobank/annotations.shtml

In signal quality change annotations "~", each non-zero bit from the four least significant bits in the subtyp
field of the annotation indicates that the corresponding signal contains noise (the least significant bit
corresponds to signal 0). The four high bits, if non-zero, indicate that the corresponding signals are unreadable
(because of very high noise amplitude, very low signal amplitude, loss of signal, or some combination of these).

WFDB_Annotation annot;
annot.time = ...;	         // annotation time, in sample intervals from the beginning of the record
annot.anntyp = NOISE;	     // annotation type (< ACMAX, see <wfdb/ecgcodes.h>
annot.subtyp = [1, 2, 3, 0]; // annotation subtype

1 - start of noise episode on channel 0, end of noise episode on channel 1
2 - start of noise episode on channel 1, end of noise episode on channel 0
3 - start of noise episode on both channels 0 and 1
0 - end of noise episode on both channels 0 and 1

EXAMPLE OUTPUT:

(NOISE detection)
Record  TPs   FN  TPp   FP  ESe E+P  DSe    D+P    DSp   Ref duration  Test duration  Test overlap
   100    0    0    0    0   -   -      -      - 100.00         0.000          0.000         0.000
   101    0    2    0    0   0   -   0.00      - 100.00      0:04.669          0.000         0.000
   103    0    1    0    0   0   -   0.00      - 100.00      0:08.653          0.000         0.000
   105   16    4   28    0  80 100  68.20  97.83  99.74      4:20.603       3:01.692      2:57.742
   106    0    0    0   40   -   0      -   0.00  98.90         0.000       0:19.742         0.000
   108    6    3   38   23  67  62  22.71  84.50  99.49      3:16.122       0:52.700      0:44.531
   109    0    0    0   24   -   0      -   0.00  99.60         0.000       0:07.139         0.000
   111    0    1    0   20   0   0   0.00   0.00  99.62      0:07.186       0:06.736         0.000
   112    2    0    3   16 100  16  30.79  47.05  99.59      0:21.042       0:13.767      0:06.478
   113    0    0    0   25   -   0      -   0.00  99.46         0.000       0:09.631         0.000
   114    2    2    3   45  50   6  13.41  29.97  99.25      0:41.869       0:18.742      0:05.617
   115    0    0    0    0   -   -      -      - 100.00         0.000          0.000         0.000
   116    0    4    0    0   0   -   0.00      - 100.00      0:25.047          0.000         0.000
   117    1    0    5    0 100 100  76.83  97.84  99.99      0:10.142       0:07.964      0:07.792
   118    2    2    5    3  50  63  26.07  82.96  99.93      0:24.381       0:07.661      0:06.356
   119    0    0    0    1   -   0      -   0.00  99.95         0.000       0:00.889         0.000
   121    0    3    0    0   0   -   0.00      - 100.00      0:17.772          0.000         0.000
   122    0    0    0    0   -   -      -      - 100.00         0.000          0.000         0.000
   123    0    0    0    0   -   -      -      - 100.00         0.000          0.000         0.000
   124    0    0    0    5   -   0      -   0.00  99.11         0.000       0:16.039         0.000
   200    6    3   21   15  67  58  38.24  82.46  99.49      1:45.717       0:49.022      0:40.425
   201    0    1    0    0   0   -   0.00      - 100.00      0:04.494          0.000         0.000
   202    0    0    0   12   -   0      -   0.00  99.64         0.000       0:06.392         0.000
   203   15    8   36  116  65  24  34.17  40.38  92.66      3:48.544       3:13.367      1:18.083
   205    0    0    0    6   -   0      -   0.00  98.56         0.000       0:25.853         0.000
   207    4    1    5   34  80  13  71.18  13.71  89.48      0:41.297       3:34.433      0:29.397
   208    3    4   20   14  43  59  22.63  68.35  99.61      1:05.211       0:21.592      0:14.758
   209    0    0    0   26   -   0      -   0.00  98.93         0.000       0:19.264         0.000
   210    1    3    1    3  25  25   9.02  44.85  99.78      0:35.225       0:07.086      0:03.178
   212    0    4    0   15   0   0   0.00   0.00  99.83      0:14.906       0:03.092         0.000
   213    0    0    0    0   -   -      -      - 100.00         0.000          0.000         0.000
   214    0    1    0    4   0   0   0.00   0.00  99.87      0:06.081       0:02.286         0.000
   215    0    2    0    0   0   -   0.00      - 100.00      0:09.536          0.000         0.000
   219    0    0    0    1   -   0      -   0.00  99.99         0.000       0:00.108         0.000
   220    0    0    0    0   -   -      -      - 100.00         0.000          0.000         0.000
   221    0    1    0    1   0   0   0.00   0.00  99.98      0:03.897       0:00.333         0.000
   222    4    2    7   75  67   9  36.85   4.68  78.43      0:50.269       6:35.925      0:18.525
   223    0    1    0    9   0   0   0.00   0.00  95.36      0:05.058       1:23.247         0.000
   228    5    5   10   20  50  33  35.82  60.71  98.63      1:40.078       0:59.056      0:35.850
   230    0    0    0    5   -   0      -   0.00  99.88         0.000       0:02.083         0.000
   231    0    0    0    0   -   -      -      - 100.00         0.000          0.000         0.000
   232    5    4    9  243  56   4  21.42   8.97  82.62      2:13.328       5:18.269      0:28.553
   233    0    0    0    0   -   -      -      - 100.00         0.000          0.000         0.000
   234    0    1    0    0   0   -   0.00      - 100.00      0:07.328          0.000         0.000

*/

#include <stdio.h>
#include <wfdb/wfdb.h>
#define map1
#define map2
#define ammap
#define mamap
#define annpos
#include <wfdb/ecgmap.h>

void init(int argc, char *argv[]);
void epicmp(unsigned int stat);
void print_results();

void find_episode(unsigned int annotator);
WFDB_Time find_overlap(unsigned int annotator);

int main(int argc, char *argv[]) {
    /* Read and interpret command-line arguments. */
    init(argc, argv);

    epicmp(0);       /* check NOISE Se */
    epicmp(1);       /* check NOISE +P */
    print_results(); /* print NOISE statistics */

    exit(0); /*NOTREACHED*/
}

char *pname; /* name by which this program was invoked */
char *record;
char *fname;
int chan; /* channel number for comparison */
int Iflag, match, mismatch;
WFDB_Time start_time, end_time, min_length, total_duration, total_overlap;
WFDB_Time ep_start[2], ep_end[2];
WFDB_Time ref_duration, ref_overlap, test_duration, test_overlap;
long STP, FN, PTP, FP;
WFDB_Anninfo an[2];

/* Perform an episode-by-episode comparison. */
void epicmp(unsigned int stat) {
    int i;
    unsigned int a, b;
    WFDB_Time duration, overlap;

    /* Return to the beginning of the annotation files. */
    if (iannsettime(0L) < 0) exit(2);

    /* If stat is 0, check sensitivity;  in this case, annotator 0 defines the
       location of episodes. */
    if (stat == 0) {
        a = 0;
        b = 1;
        ref_duration = ref_overlap = STP = FN = 0L;
    }
    /* Otherwise, check positive predictivity;  in this case, annotator 1
       defines the location of episodes. */
    else {
        a = 1;
        b = 0;
        test_duration = test_overlap = PTP = FP = 0L;
    }

    /* Initialize state variables. */
    match = mismatch = 0;
    total_duration = total_overlap = 0L;
    ep_start[0] = ep_start[1] = ep_end[0] = ep_end[1] = 0L;

    /* Find the first annotator a episode that ends during the test period. */
    do {
        find_episode(a);
    } while (0L < ep_end[a] && ep_end[a] < start_time);

    /* Quit immediately if there are no such episodes. */
    if (ep_end[a] == 0L) return;

    /* Adjust the starting time of the annotator a episode if it begins during
       the learning period. */
    if (ep_start[a] < start_time) ep_start[a] = start_time;

    /* Find the first annotator b episode. */
    find_episode(b);

    /* Process an annotator a episode for each iteration of this loop. */
    while (ep_end[a] && (end_time == 0L || ep_start[a] < end_time)) {
        /* Adjust the episode ending time if it ends after the end of the
           test period. */
        if (end_time > 0L && ep_end[a] > end_time) ep_end[a] = end_time;

        duration = ep_end[a] - ep_start[a];

        /* If the episode is too short, ignore it. */
        if (duration < min_length) {
            find_episode(a);
            continue;
        }

        /* Add the duration of this episode to the tally. */
        total_duration += duration;

        /* Get the duration of overlap;  update the count of matching
           episodes if appropriate, and update the overlap tally. */
        if ((overlap = find_overlap(b)) > 0L) {
            match++;
            total_overlap += overlap;
        }
        /* If there was no overlap, update the count of mismatched episodes. */
        else {
            mismatch++;
        }

        /* Get the next episode. */
        find_episode(a);
    }

    if (a == 0) {
        STP = match;
        FN = mismatch;
        ref_duration = total_duration;
        ref_overlap = total_overlap;
    } else {
        PTP = match;
        FP = mismatch;
        test_duration = total_duration;
        test_overlap = total_overlap;
    }
}

int lflag;
WFDB_Time min_length;

void find_episode(unsigned int annotator) {
    int stat, stcount = 0;
    WFDB_Time tt;
    static WFDB_Annotation annot;

    ep_start[annotator] = ep_end[annotator] = 0L;

    /* Find beginning of episode. */
    while ((stat = getann(annotator, &annot)) == 0) {
        if (annot.anntyp == NOISE) {
            if ((annot.subtyp & (1 << chan)) != 0 || (annot.subtyp & (1 << (chan + 4))) != 0) {
                ep_start[annotator] = annot.time;
                break;
            }
        }
    }

    /* Quit if at end of annotation file. */
    if (stat != 0) return;
    tt = annot.time;

    /* Find end of episode. */
    while ((stat = getann(annotator, &annot)) == 0) {
        tt = annot.time;
        if (annot.anntyp == NOISE) {
            if ((annot.subtyp & (1 << chan)) == 0 && (annot.subtyp & (1 << (chan + 4))) == 0) {
                ep_end[annotator] = annot.time;
                break;
            }
        }
    }

    /* If end of annotation file was reached, episode did not terminate --
       assume that it ended at the greater of tt (the time of the previous
       annotation) and end_time. */
    if (stat != 0) ep_end[annotator] = (end_time > tt) ? end_time : tt;
}

/* This function determines the duration of any episodes of the specified
   type according to the specified annotator within a window defined by
   ep_start[1-annotator] and ep_end[1-annotator]. */

WFDB_Time find_overlap(unsigned int annotator) {
    WFDB_Time overlap = 0L, o_start, o_end;

    /* Return immediately if there are no more episodes. */
    if (ep_end[annotator] == 0L) return (overlap);

    /* If the current episode ended before the beginning of the window, get
       another. */
    while (ep_end[annotator] < ep_start[1 - annotator]) {
        find_episode(annotator);
        if (ep_start[annotator] == 0L) return (overlap);
    }

    /* If the current episode starts after the end of the window, return
       immediately. */
    if (ep_start[annotator] > ep_end[1 - annotator]) return (overlap);

    /* There is at least some overlap -- determine its extent. */
    do {
        /* Ignore any episodes shorter than the minimum length if using -I */
        if (Iflag && ep_end[annotator] - ep_start[annotator] < min_length) {
            find_episode(annotator);
            continue;
        }

        /* Ignore any portion of the current episode that precedes the
           beginning of the window. */
        if (ep_start[annotator] < ep_start[1 - annotator]) {
            o_start = ep_start[1 - annotator];
        } else {
            o_start = ep_start[annotator];
        }

        /* Ignore any portion of the current episode that follows the end of
           the window. */
        if (ep_end[annotator] > ep_end[1 - annotator]) {
            o_end = ep_end[1 - annotator];
        } else {
            o_end = ep_end[annotator];
        }

        /* Tally the overlap. */
        overlap += o_end - o_start;

        /* If the current episode ends after the end of the window, stop. */
        if (ep_end[annotator] >= ep_end[1 - annotator]) break;
        /* Otherwise, get the next episode and see if it also falls in the
           window. */
        find_episode(annotator);
    } while (0L < ep_start[annotator] && ep_start[annotator] < ep_end[1 - annotator]);
    return (overlap);
}

FILE *ofile;

/* `pstat' prints a statistic described by s, defined as the quotient of a and
   b expressed in percentage units.  Undefined values are indicated by `-'. */

void pstat(char *s, long a, long b) {
    if (!lflag) {
        (void)fprintf(ofile, "%s:", s);
        if (b <= 0) {
            (void)fprintf(ofile, "   - ");
        } else {
            (void)fprintf(ofile, " %3d%%", (int)((100. * a) / b + 0.5));
        }
        (void)fprintf(ofile, " (%ld/%ld)\n", a, b);
    } else {
        if (b <= 0) {
            (void)fprintf(ofile, "   -");
        } else {
            (void)fprintf(ofile, " %3d", (int)((100. * a) / b + 0.5));
        }
    }
}

/* `tstat' prints a statistic as above, but prints the numerator and
   denominator as times. */

char *zmstimstr(WFDB_Time t) {
    return (t ? mstimstr(t) : "       0.000");
}

char *lzmstimstr(WFDB_Time t) {
    char *p = zmstimstr(t);
    while (*p == ' ') {
        p++;
    }
    return (p);
}

void tstat(char *s, WFDB_Time a, WFDB_Time b) {
    if (!lflag) {
        (void)fprintf(ofile, "%s:", s);
        if (b <= 0) {
            (void)fprintf(ofile, "   - ");
        } else {
            (void)fprintf(ofile, " %3d%%", (int)((100. * a) / b + 0.5));
        }
        (void)fprintf(ofile, " (%s", lzmstimstr(a));
        (void)fprintf(ofile, "/%s)\n", lzmstimstr(b));
    } else {
        if (b <= 0) {
            (void)fprintf(ofile, "      -");
        } else {
            (void)fprintf(ofile, " %6.2f", ((100. * a) / b));
        }
    }
}

void print_results() {
    char *ofname;
    ofname = fname;

    /* Open output file.  If line-format output was selected, write column
       headings only if the file must be created from scratch. */
    if (strcmp(ofname, "-")) {
        if ((ofile = fopen(ofname, "r")) == NULL) {
            if ((ofile = fopen(ofname, "w")) == NULL) {
                (void)fprintf(stderr, "%s: can't create %s\n", pname, ofname);
                exit(3);
            }
            if (lflag) {
                (void)fprintf(ofile, "(NOISE detection)\n");
                (void)fprintf(ofile, "Record  TPs   FN  TPp   FP");
                (void)fprintf(ofile, "  ESe E+P  DSe    D+P    DSp   Ref duration  Test duration  Test overlap\n");
            }
        } else {
            (void)fclose(ofile);
            if ((ofile = fopen(ofname, "a")) == NULL) {
                (void)fprintf(stderr, "%s: can't modify %s\n", pname, ofname);
                exit(3);
            }
        }
    } else {
        ofile = stdout;
    }

    if (lflag) {
        (void)fprintf(ofile, "%6s %4ld %4ld %4ld %4ld", record, STP, FN, PTP, FP);
    } else {
        (void)fprintf(ofile, "NOISE episode-by-episode comparison results for record %s\n", record);
        (void)fprintf(ofile, "Reference annotator: %s\n", an[0].name);
        (void)fprintf(ofile, "     Test annotator: %s\n\n", an[1].name);
    }
    pstat("           Episode sensitivity", STP, STP + FN);
    pstat(" Episode positive predictivity", PTP, PTP + FP);
    tstat("          Duration sensitivity", ref_overlap, ref_duration);
    tstat("Duration positive predictivity", test_overlap, test_duration);
    // TN / (TN + FP)
    if (end_time > 0) {
        WFDB_Time window = end_time - start_time;
        WFDB_Time tn = window - (ref_duration + test_duration - test_overlap);
        WFDB_Time fp = (test_duration - test_overlap);
        tstat("          Duration specificity", tn, tn + fp);
    } else {
        tstat("          Duration specificity", 0, 0);
    }
    if (!lflag) (void)fprintf(ofile, "Duration of reference episodes:");
    (void)fprintf(ofile, "  %s", zmstimstr(ref_duration));
    if (!lflag) {
        (void)fprintf(ofile, "\n     Duration of test episodes:");
    } else {
        (void)fprintf(ofile, " ");
    }
    (void)fprintf(ofile, "  %s", zmstimstr(test_duration));

    if (lflag) {
        (void)fprintf(ofile, "  %s\n", zmstimstr(test_overlap));
    } else {
        (void)fprintf(ofile, "\n");
    }

    if (ofile != stdout) (void)fclose(ofile);
}

char *prog_name(char *s) {
    char *p = s + strlen(s);

#ifdef MSDOS
    while (p >= s && *p != '\\' && *p != ':') {
        if (*p == '.') *p = '\0';                    /* strip off extension */
        if ('A' <= *p && *p <= 'Z') *p += 'a' - 'A'; /* convert to lower case */
        p--;
    }
#else
    while (p >= s && *p != '/')
        p--;
#endif
    return (p + 1);
}

static char *help_strings[] = {"usage: %s -r RECORD -a REF TEST [OPTIONS ...]\n",
                               "where RECORD is the record name;  REF is reference annotator name;  TEST is",
                               "the test annotator name; and OPTIONS may include any of:",
                               " -f TIME        begin comparison at specified TIME (default: 5 minutes",
                               "                 after beginning of record)",
                               " -h             print this usage summary",
                               " -i TIME        exclude episodes shorter than TIME from episode statistics",
                               " -I TIME	  exclude episodes shorter than TIME from all statistics",
                               " -l             write reports in line format (default: matrix format)",
                               " -L             same as -l",
                               " -t TIME        stop comparison at specified TIME (default: end of record",
                               "                 if defined, end of reference annotation file otherwise;",
                               "                 if TIME is 0, the comparison ends when the end of either",
                               "                 annotation file is reached)",
                               " -N FILE        append NOISE reports to FILE",
                               " -c chn         specify the channel number for comparison",
                               NULL};

void help() {
    int i;
    (void)fprintf(stderr, help_strings[0], pname);
    for (i = 1; help_strings[i] != NULL; i++)
        (void)fprintf(stderr, "%s\n", help_strings[i]);
}

void init(int argc, char *argv[]) {
    int i;

    chan = 0; /* default channel number for comparison */

    pname = prog_name(argv[0]);
    for (i = 1; i < argc; i++) {
        if (*argv[i] == '-') switch (*(argv[i] + 1)) {
            case 'a': /* annotator names follow */
                if (++i >= argc - 1) {
                    (void)fprintf(stderr, "%s: reference and test annotator names must follow -a\n", pname);
                    exit(1);
                }
                an[0].name = argv[i];
                an[1].name = argv[++i];
                break;
            case 'f': /* start time follows */
                if (++i >= argc) {
                    (void)fprintf(stderr, "%s: start time must follow -f\n", pname);
                    exit(1);
                }
                start_time = i; /* save arg index, convert to samples later,
                           when record has been opened and sampling
                           frequency is known */
                break;
            case 'h': /* print usage summary */
                help();
                exit(1);
                break;
            case 'I': /* ignore short intervals when calculating overlap */ Iflag = 1; /* fall through to -i */
            case 'i':                                                                  /* minimum episode length follows */
                if (++i >= argc) {
                    (void)fprintf(stderr, "%s: minimum episode length must follow %s\n", pname, argv[i - 1]);
                    exit(1);
                }
                min_length = i;
                break;
            case 'l': /* line-format output */
            case 'L': /* treat -L and -l the same way, for
                     consistency with `bxb' and `rxr' usage */
                lflag = 1;
                break;
            case 'r': /* record name follows */
                if (++i >= argc) {
                    (void)fprintf(stderr, "%s: record name must follow -r\n", pname);
                    exit(1);
                }
                record = argv[i];
                break;
            case 't': /* end time follows */
                if (++i >= argc) {
                    (void)fprintf(stderr, "%s: end time must follow -t\n", pname);
                    exit(1);
                }
                end_time = i;
                break;
            case 'N': /* perform NOISE comparison */
                if (++i >= argc) {
                    (void)fprintf(stderr, "%s: output file name must follow -N\n", pname);
                    exit(1);
                }
                fname = argv[i];
                break;
            case 'c': /* specify the channel number for comparison */
                if (++i >= argc) {
                    (void)fprintf(stderr, "%s: channel number must follow -c\n", pname);
                    exit(1);
                }
                chan = atoi(argv[i]);
                break;
            default: (void)fprintf(stderr, "%s: unrecognized option %s\n", pname, argv[i]); exit(1);
            }
        else {
            (void)fprintf(stderr, "%s: unrecognized argument %s\n", pname, argv[i]);
            exit(1);
        }
    }

    if (!record || !an[0].name) {
        help();
        exit(1);
    }

    if (start_time != 0L || end_time != 0L || min_length != 0) (void)fprintf(stderr, "%s: (warning) nonstandard comparison selected\n", pname);

    if (sampfreq(record) <= 0) {
        (void)fprintf(stderr, "%s: (warning) %g Hz sampling frequency assumed\n", pname, WFDB_DEFFREQ);
        (void)setsampfreq(WFDB_DEFFREQ);
    }

    /* Set the minimum episode length and the times of the start and end of
       the test period. */
    if (min_length) min_length = strtim(argv[min_length]);
    if (start_time) {
        start_time = strtim(argv[(int)start_time]);
        if (start_time < (WFDB_Time)0) start_time = -start_time;
    } else {
        start_time = strtim("5:0"); /* 5 minutes */
    }
    if (end_time) {
        end_time = strtim(argv[(int)end_time]);
        if (end_time < (WFDB_Time)0) end_time = -end_time;
    } else if ((end_time = strtim("e")) == 0L)
        end_time = -1L; /* record length unavailable -- go to end of
                   reference annotation file */
    if (end_time > 0L && end_time < start_time) {
        (void)fprintf(stderr, "%s: improper interval specified\n", pname);
        exit(1);
    }
    an[0].stat = an[1].stat = WFDB_READ;

    /* Open annotation files. */
    if (annopen(record, an, 2) < 0) exit(2);
}
