//March 29th, 2021
//CIS*3490 Assignment 4 - Connor Todd (1039174)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <math.h>


#define MAXTABLE 1000

struct bNode {
    char *word;
    double sum;
    struct bNode *left;
    struct bNode *right;
} typedef bNode;

bNode *newNode(char *w);
bNode *insertNode(bNode *tree, char*);
void deleteTree(bNode *tree);
bNode *searchTree(bNode *tree, char *);
void calcProbabilties(bNode*, int *, int, double *, char **);
bNode * optimalTree(int, int, char**);
bNode *insertNodeProbability(bNode *, char *, double);
bNode *newNodeProbabability(char *, double);
bNode *greedyTree(char **, double *, int, int);