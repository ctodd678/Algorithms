#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <math.h>
#include "header.h"

//*GLOBAL VARIABLES FOR TABLES
double avg_table[MAXTABLE][MAXTABLE];
int root_table[MAXTABLE][MAXTABLE];

//March 29th, 2021
//CIS*3490 Assignment 4 - Connor Todd (1039174)

int main(){

    FILE *fp1;
    int pNodeSize = 1;
    char file1[128];
    char *fileStr;
    long length = 0;
    char *word;
    int numWords = 0;
    int i = 0;
    int j = 0;

    double *P_Array = malloc(sizeof(double) * MAXTABLE);  //array of probabilities
    char **uniq_words = malloc(sizeof(char*) * MAXTABLE); //array of unique words

    //initialize unqiue word array
    for(int i = 0; i < MAXTABLE; i++) {
        uniq_words[i] = malloc(sizeof(char) * 64);  
    }

    //initialize tables
    for (i = 0; i < MAXTABLE; i++){
        for (j = 0; j < MAXTABLE;j++){
            avg_table[i][i] = 0;
            root_table[i][i] = 0;
        }
    }

    //*GET USER INPUT
    printf("Please enter the name of the file:\n");
    scanf("%s" , file1);

    if(!(fp1 = fopen(file1, "r"))) {
        printf("ERROR! Cannot open file %s.", file1);
        exit(1);
    }

    /*QUESTION 1
    * Uses dynamic programming, faster compared to greedy.
    * However, may not always get the optimal value.
    */
    printf("----QUESTION 1----\n");
    char word1[32], word2[32];
    printf("Enter a key: ");
    scanf("%s", word1);

    fseek(fp1, 0, SEEK_END);
    length = ftell(fp1);
    fseek(fp1, 0, SEEK_SET);

    fileStr = malloc(sizeof(char) * length);
    strcpy(fileStr, "");

    if(fileStr == NULL) {
        printf("ERROR! Failed malloc (fileStr).\n");
        exit(1);
    }

    fread(fileStr, 1, length, fp1);
    fileStr[length - 1] = '\0';

    //*CREATE BINARY TREE W/ WORDS FROM FILE
    bNode * root_node = NULL;
    word = strtok(fileStr, " ");
    while(word != NULL) {

        word = strtok(NULL, " \n\t");
        if(word == NULL) {
            break;
        }
        root_node = insertNode(root_node, word);
        numWords++;
    }

    //*TRAVERSE TREE, ADD NODES TO PROBABILTIY ARRAY & CREATE ARRAY OF UNIQUE WORDS
    calcProbabilties(root_node, &pNodeSize, numWords,  P_Array, uniq_words);

    //*DYNAMIC PROGRAMMING OPTIMAL BST ALGORITM (FROM TEXTBOOK pg 302)
    pNodeSize--;
    int k = 0;
    int d = 0;
    int s = 0;
    int k_min = 0;
    double min_val = 0;
    double sum = 0;

    for (i = 1; i <= pNodeSize; i++) {
        avg_table[i][i - 1] = 0;
        avg_table[i][i] = P_Array[i];
        root_table[i][i] = i;
    }

    for (d = 1; d <= pNodeSize - 1; d++) {
        for (i = 1; i <= pNodeSize - d; i++) {
            j = i + d;
            min_val = 999999999999;    //very high number to represent infinity
            for (k = i; k <= j; k++) {
                if ((avg_table[i][k - 1] + avg_table[k + 1][j]) < min_val) {
                    min_val = avg_table[i][k - 1] + avg_table[k + 1][j];
                    k_min = k;
                }
            }
            root_table[i][j] = k_min;
            sum =  P_Array[i];
            for (s = i + 1; s <= j; s++) {
                sum = sum + P_Array[s];
            }
            avg_table[i][j] = min_val + sum;
        }
    }

    //*MAKE OPTIMAL TREE AND SEARCH FOR WORD
    bNode *root_optimal = NULL;
    root_optimal = optimalTree(1, pNodeSize, uniq_words);
    searchTree(root_optimal, word1);
    
    /*QUESTION 2
    * Uses greedy algoritm.
    * Slower versus dynamic algorithm.
    */
    printf("\n----QUESTION 2----\n");
    printf("Enter a key: ");
    scanf("%s", word2);

    //*MAKE GREEDY TREE AND SEARCH FOR WORD
    bNode *root_greedy = NULL;
    root_greedy = greedyTree(uniq_words, P_Array, 0, pNodeSize);
    searchTree(root_greedy, word2);
    

    //*FREE EVERYTHING AND EXIT
    deleteTree(root_node);
    deleteTree(root_optimal);
    deleteTree(root_greedy);

    free(P_Array);
    for(int i = 0; i < MAXTABLE; i++) {
        free(uniq_words[i]);
    }
    free(uniq_words);
    free(fileStr);

    fclose(fp1);

    return 0;
}

bNode *newNode(char *w) {

    bNode* temp = malloc(sizeof(bNode));
    temp->word = malloc(sizeof(char) * strlen(w) + 1);

    strcpy(temp->word, w);
    temp->sum = 1;
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}

//*ADDS NODES TO TREE SORTED BY WORDS
bNode *insertNode(bNode *tree, char* w) {

    bNode *temp = tree;
    int cmp;
    
    if(tree != NULL) {
        cmp = strcmp(w, temp->word);
        if(cmp == 0) 
            temp->sum = temp->sum + 1;
        else if(cmp < 0)
            tree->left = insertNode(temp->left, w);
        else
            temp->right = insertNode(temp->right, w);
    }
    else {
        return newNode(w);
    }

    return tree;
}

bNode *searchTree(bNode *tree, char *w) {
    int cmp;

    if(tree == NULL) {
        printf("No matches for %s.\n", w);
        return tree;
    }
    else {
        if(w != NULL) {
            cmp = strcmp(w, tree->word);
        }
        if(cmp == 0)  {
            printf("Compared with %s (%.3f) found.\n", tree->word, tree->sum);
        }
        else if(cmp < 0) {
            printf("Compared with %s (%.3f) go left subtree.\n", tree->word, tree->sum);
            tree->left = searchTree(tree->left, w);
        }
        else {
            printf("Compared with %s (%.3lf) go right subtree.\n", tree->word, tree->sum);
            tree->right = searchTree(tree->right, w);
        }
    }

    return tree;
}

void deleteTree(bNode *tree) {

    if(tree != NULL) {
        deleteTree(tree->left);
        deleteTree(tree->right);
        free(tree->word);
        free(tree);
    }
}

//*INORDER TRAVERSAL AND CALCULATE PROBABILTIES
void calcProbabilties(bNode* currNode, int *size, int nWords, double *probs, char **u_w) {
    if (currNode == NULL)
        return;
    
    if(currNode->left != NULL)
        calcProbabilties(currNode->left, size, nWords, probs, u_w);

    strcpy(u_w[*size], currNode->word); //add word to unique words array
    probs[*size] = (double)currNode->sum / (double)nWords;  //set probabilties
    *size = *size + 1;

    if(currNode->right != NULL)
        calcProbabilties(currNode->right, size, nWords, probs, u_w);
}

bNode * optimalTree(int i, int j, char** u_w){

	bNode * temp;

	if(i > j) {
        temp = NULL;
    }
	else {
        temp = malloc(sizeof(bNode));
        temp->word = malloc(sizeof(char) * strlen(u_w[root_table[i][j]]) + 1);
        strcpy(temp->word, u_w[root_table[i][j]]);
        temp->sum = avg_table[i][j];

        //*MAKE LEFT AND RIGHT TREES
		temp->left = optimalTree(i, root_table[i][j] - 1, u_w);
		temp->right = optimalTree(root_table[i][j] + 1, j, u_w); //right subtree
	}

	return temp;
}

bNode * greedyTree(char **words, double *p, int i, int j){

	bNode * temp;
    if(i > j) {
        temp = NULL;
    }
	else {
        //*FIND MAX PROBABILITY INDEX
        double max = 0;
        int maxI = 0;
        for(int k = i; k <= j; k++) {
            if(p[k] > max) {
                max = p[k];
                maxI = k;
            }
        }

        temp = malloc(sizeof(bNode));
        temp->word = malloc(sizeof(char) * strlen(words[maxI]) + 1);
        strcpy(temp->word, words[maxI]);
        temp->sum = p[maxI];

        //*MAKE LEFT AND RIGHT TREES
		temp->left = greedyTree(words, p, i, maxI - 1);
		temp->right = greedyTree(words, p, maxI + 1, j);
	}

	return temp;
}