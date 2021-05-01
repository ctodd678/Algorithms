#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <math.h>
#include "header.h"

int main(){

    FILE *fp1, *fp2;
    char file1[128];
    char file2[128];
    char *file2Str;
    long length = 0;

    char **nums;
    char inputStr[50];
    char fileNum[64];
    int i = 0, l = 0, anaCount = 0;
    float excTime;

    int horsTable[MAXTABLE];

    struct timeb start, end;
    printf("Please enter the name for file for question 1:\n");
    scanf("%s" , file1);

    nums = malloc(sizeof(char*) * MAXNUMS);


    if(!(fp1 = fopen(file1, "r"))) {
        printf("ERROR! Cannot open file %s.", file1);
        exit(1);
    }

    for(i = 0; i < MAXNUMS; i++) {
        fscanf(fp1, "%s", fileNum);
        nums[i] = malloc(sizeof(char) * strlen(fileNum));
        strcpy(nums[i], fileNum);
    }
    printf("QUESTION 1\n");
    printf("Please enter a string (of numbers) to search for anagrams of:\n");
    scanf("%s", inputStr);

    //*PART 1.1
    ftime(&start);
    anaCount = p11(nums, inputStr);
    ftime(&end);

    excTime = (1000 * (end.time - start.time)) + (end.millitm - start.millitm);
    printf("\nP1.1 Execution Time: %.3fms | %.4fs\n", excTime, (excTime / 1000));
    printf("P1.1 Anagram Count: %d", anaCount);

    printf("\n\n");

    //*PART 1.2
    for(i = 0; i < MAXNUMS; i++) {
        qsort(nums[i], strlen(nums[i]), sizeof(char), cmp);
    }

    
    qsort(nums, MAXNUMS, sizeof(char*), cmp2);


    // for(i = 0; i < 10; i++) {
    //     printf("%s\n", nums[i]);
    // }


    ftime(&start);
    anaCount = p12(nums, inputStr);
    ftime(&end);

    excTime = (1000 * (end.time - start.time)) + (end.millitm - start.millitm);
    printf("\nP1.2 Execution Time: %.3fms | %.4fs\n", excTime, (excTime / 1000));
    printf("P1.2 Anagram Count: %d\n\n", anaCount);

    
    //*PART 2
    printf("Please enter the name for file for question 2:\n");
    scanf("%s" , file2);

    fp2 = fopen(file2, "r");

    fseek(fp2, 0, SEEK_END);
    length = ftell(fp2);
    fseek(fp2, 0, SEEK_SET);

    file2Str = malloc(sizeof(char) * length);
    strcpy(file2Str, "");

    if(file2Str == NULL) {
        printf("ERROR! Failed malloc (file2Str).\n");
        exit(1);
    }

    fread(file2Str, 1, length, fp2);
    file2Str[length - 1] = '\0';

    //*PART 2.1
    printf("QUESTION 2\n");
    printf("Please enter a word to search for matches of:\n");
    scanf("%s", inputStr);
    strcat(inputStr, "\0");

    ftime(&start);
    anaCount = p21(file2Str, inputStr);
    ftime(&end);

    excTime = (1000 * (end.time - start.time)) + (end.millitm - start.millitm);
    printf("\nP2.1 Execution Time: %.3fms | %.4fs\n", excTime, (excTime / 1000));
    printf("P2.1 Pattern Occurance Count: %d\n\n", anaCount);

    int shifts = 0;


    //*PART 2.2
    l = strlen(inputStr);

    for(i = 0; i < 512; i++)
        horsTable[i] = l;

    for(i = 0; i < l - 1; i++)
        horsTable[inputStr[i]] = l - i - 1;

    int horsPos = 0;
    anaCount = 0;


    horsPos = p22(file2Str, inputStr, horsTable, 0, &shifts);
    ftime(&start);
    while((horsPos = p22(file2Str, inputStr, horsTable, horsPos, &shifts)) != - 1) {
        horsPos = horsPos + 1;
        anaCount++;
    }
    ftime(&end);

    printf("P2.2 Shifts: %d", shifts);
    excTime = (1000 * (end.time - start.time)) + (end.millitm - start.millitm);
    printf("\nP2.2 Execution Time: %.3fms | %.4fs\n", excTime, (excTime / 1000));
    printf("P2.2 Pattern Occurance Count: %d\n\n", anaCount);

    //*PART 2.3
    l = strlen(file2Str);
    int boyerTable[512];

    //SHIFT TABLE 
    for (i = 0; i < 512; i++) 
        boyerTable[i] = -1; 

    for (i = 0; i < strlen(inputStr); i++) 
        boyerTable[(int)inputStr[i]] = i; 

    ftime(&start);
    anaCount = p23(file2Str, inputStr, boyerTable, 0, &shifts);

    ftime(&end);
    excTime = (1000 * (end.time - start.time)) + (end.millitm - start.millitm);
    printf("\nP2.3 Execution Time: %.3fms | %.4fs\n", excTime, (excTime / 1000));
    printf("P2.3 Pattern Occurance Count: %d\n", anaCount);


    for(i = 0; i < MAXNUMS; i++) {
       free(nums[i]);
    }

    free(file2Str);
    free(nums);
    
    fclose(fp1);
    fclose(fp2);

    return 0;
}

//BRUTE FORCE FOR Q1.1
int p11(char **nums, char* inputStr) {
    int i;
    int *inputOccurances;
    int *cmpOccurances;
    int numAnagrams = 0;

    

    inputOccurances = findOccurances(inputStr);


    for(i = 0; i < MAXNUMS; i++) {
        cmpOccurances = findOccurances(nums[i]);

        if(strcmp(inputStr, nums[i]) == 0)
            continue;
        if(isSameOccurances(inputOccurances, cmpOccurances)) {
            printf("%s\n", nums[i]);
            numAnagrams++;
        }
    }

  return numAnagrams;
}

//CHECKS IF TWO STRINGS HAVE SAME OCCURANCES
int isSameOccurances(int *o1, int *o2) {
    int i = 0;
    int isOccurance = 1;

    for(i = 0; i < 10; i++) {

        if(o1[i] != o2[i]) {
            isOccurance = 0;
            break;
        }
    }

    return isOccurance;
}

//GETS NUMBER OF OCCURANCES FOR EACH CHARACTER IN A STRING
int *findOccurances(char *num) {

    int *occurances = malloc(sizeof(int) * 10);
    int i = 0; 

    for(i = 0; i < 10; i++)
        occurances[i] = 0;

    for(i = 0; i < strlen(num); i++) {
        occurances[(int)(num[i]) - 48] = occurances[(int)(num[i]) - 48] + 1;
    }

    return occurances;
}

//PRESORT BRUTE FORCE FOR Q1.2
int p12(char **nums, char* inputStr) {
    int i;
    int *inputOccurances;
    int *cmpOccurances;
    int numAnagrams = 0;

    //qsort(inputStr, strlen(inputStr), sizeof(char), cmp);
    inputOccurances = findOccurances(inputStr);
    //printf("%s\n", inputStr);
    // qsort(inputStr, strlen(inputStr), sizeof(char), cmp);

    for(i = 0; i < MAXNUMS; i++) {
        cmpOccurances = findOccurances(nums[i]);

        if(strcmp(inputStr, nums[i]) == 0)
            continue;
        if(isSameOccurances(inputOccurances, cmpOccurances)) {
            printf("%s\n", nums[i]);
            numAnagrams++;
        }
    }
    if(numAnagrams != 0)
        numAnagrams--;  //subtract the orginal string

    return numAnagrams;
}

//BRUTE FORCE FOR Q2.1
int p21(char *fileStr, char* inputStr) {

    int i = 0;
    int inputI = 0;
    int tempI = 0;
    int same = 0;
    int shifts = 0;
    int occuranceCount = 0;

    while(fileStr[i] != '\0') {
        same = 0;
        tempI = i;

        for(int j = 0; j < strlen(inputStr); j++) {

            if(fileStr[i] == inputStr[j]) {
                same = same + 1;
                i++;
            }
            else {
                i = tempI;
                shifts = shifts + 1;
                break;
            }

            if(same == strlen(inputStr)) {
                occuranceCount = occuranceCount + 1;
                i = tempI;
                break;
            }
        }

        i++;
    }

    printf("P2.1 Shifts: %d", shifts);

    return occuranceCount;
}

//HORSPOOL FOR Q2.2
int p22(char *fileStr, char* inputStr, int *table, int startIndex, int *s) {

    int i;
    int k = 0;
    int m;
    m = strlen(inputStr);
    i = (m - 1) + startIndex;

    while(fileStr[i] != '\0') {
        k = 0;
        while((k < m) && (inputStr[m - 1 - k] == fileStr[i - k])) {
            k++;
        }
        if(k == m)
            return i - m + 1;
        else
            i = i + table[(int)fileStr[i]];
        
        *s = *s + 1;
    } 

    return -1;
}

//HORSPOOL FOR Q2.3
int p23(char *fileStr, char* inputStr, int *table, int startIndex, int *s) {
    
    int m = strlen(inputStr);
    int n = strlen(fileStr);
    int shift = 0;
    int shiftCounter = 0;
    int occurances = 0;

    while(shift <= (n - m)) { 
        int j = m - 1; 

        while(j >= 0 && inputStr[j] == fileStr[shift + j]) {
            j--;
        }
    
        if (j < 0) {
            occurances = occurances + 1;
            if(n >= shift + m)
                shift = shift + m;
            else
                shift = shift + (m - table[(int)fileStr[shift + m]]); 
        } 
        else {
            if(j - table[(int)fileStr[shift + j]] >= 1) {
                shift = shift + (j - table[(int)fileStr[shift + j]]);
            }
            else {
                shift = shift + 1;
            }
        }
        shiftCounter++;
    } 

    printf("P2.3 Shifts: %d", shiftCounter);

    return occurances;

}

int cmp (const void * a, const void * b) {

    return *(char*)b - *(char*)a;
}

int cmp2 (const void * a, const void * b) {

    return atol(*(const char**)a) - atol(*(const char**)b);
}
