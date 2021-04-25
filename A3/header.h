//March 15th, 2021
//CIS*3490 Assignment 3 - Connor Todd (1039174)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <math.h>

#define MAXNUMS 30000   //maximum number of integers in data file
#define MAXPOINTS 30000 //maximum number of convex in data file
#define MAXTABLE 300000

int     p11                 (char **, char *);
int     p12                 (char **, char *);
int     p21                 (char *, char*);
int     p22                 (char *, char* , int *, int, int *);
int     p23                 (char *, char*, int *, int, int *);
int     cmp                 (const void *, const void *);
int     cmp2                (const void *, const void *);
int    *findOccurances      (char *);
int     isSameOccurances    (int *, int *);