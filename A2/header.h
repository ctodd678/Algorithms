//Feb 22nd, 2021
//CIS*3490 Assignment 2 - Connor Todd (1039174)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <math.h>

#define MAXNUMS 50000   //maximum number of integers in data file
#define MAXPOINTS 30000 //maximum number of convex in data file

//*POINT STRUCTURE FOR PART 2
struct Point {
    double x;
    double y;
}typedef Point;

int     p11             (int[]);
int     p12             (int [], int, int);
int     p21             (Point *);
int     p22             (Point *, int, int);
void    p22_quick       (Point *, Point *, Point, Point, int);
int     getSide         (Point, Point, Point);
int     addPoint        (Point *, double, double);
int     countPoints     (Point *p);
void    printPoints     (Point[], int);
int     cmp             (const void *, const void *);