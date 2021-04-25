#include "header.h"

//Feb 22nd, 2021
//CIS*3490 Assignment 2 - Connor Todd (1039174)

int main(){

    FILE *fp1, *fp2;
    char file1[128] = "data_1.txt";
    char file2[128] = "data_2.txt";

    int nums[MAXNUMS];
    int i = 0, invCount = 0, min, max;
    float excTime;
    Point convex[MAXPOINTS];

    struct timeb start, end;

    fp1 = fopen(file1, "r");
    if(fp1 == NULL) {
        printf("\nERROR! Opening file.");
    }

    for(i = 0; i < MAXNUMS; i++)
        fscanf(fp1, "%d", &nums[i]);

    fp2 = fopen(file2, "r");
    if(fp2 == NULL) {
        printf("\nERROR! Opening file.");
    }

    for(i = 0; i < MAXPOINTS; i++)
        fscanf(fp2, "%lf %lf", &convex[i].x, &convex[i].y);


    //*PART 1.1
    ftime(&start);
    invCount = p11(nums);
    ftime(&end);

    excTime = (1000 * (end.time - start.time)) + (end.millitm - start.millitm);
    printf("\nP1.1 Execution Time: %.3fms | %.4fs\n", excTime, (excTime / 1000));
    printf("P1.1 Inversion Count: %d\n", invCount);
    printf("P1.1 Efficiency: n^2\n");

    //*PART 1.2
    ftime(&start);
    invCount = p12(nums, 0, MAXNUMS - 1);
    ftime(&end);

    excTime = (1000 * (end.time - start.time)) + (end.millitm - start.millitm);
    printf("\nP1.2 Execution Time: %.2fms | %.4fs\n", excTime, (excTime / 1000));
    printf("P1.2 Inversion Count: %d\n\n", invCount);
    printf("P1.2 Efficiency: nlog(n)\n");


    //*PART 2.1
    printf("P2.1 Pairs:\n");
    ftime(&start);
    invCount = p21(convex);
    ftime(&end);

    excTime = (1000 * (end.time - start.time)) + (end.millitm - start.millitm);
    printf("\nP2.1 Execution Time: %.3fms | %.4fs\n", excTime, (excTime / 1000));
    printf("P2.1 Extreme Points: %d\n\n", invCount);
    printf("P2.2 Efficiency: n^3\n");


    //*PART 2.2

    //*GET MIN AND MAX X COORDS
    for(i = 0; i < MAXPOINTS; i++) {
        if(convex[i].x > convex[max].x)
            max = i;
        if(convex[i].x < convex[min].x)
            min = i;
    }

    printf("P2.2 Pairs:\n");
    ftime(&start);
    invCount = p22(convex, min, max);
    ftime(&end);

    excTime = (1000 * (end.time - start.time)) + (end.millitm - start.millitm);
    printf("\nP2.2 Execution Time: %.3fms | %.4fs\n", excTime, (excTime / 1000));
    printf("P2.2 Extreme Points: %d\n", invCount);
    printf("P2.2 Efficiency: nlog(n)\n");


    fclose(fp1);
    fclose(fp2);

    return 0;
}

int p11(int array[]) {

    int i = 0, j = 0, inv = 0;
    int size = MAXNUMS;

    for(i = 0; i < size; i++)
        for(j = i; j < size; j++) 
            if(array[j] < array[i])
                inv++;

    return inv;
}

int p12(int array[], int left, int right) {

    //*MERGESORT
    int inv = 0, i = 0, j = 0, tempMid = 0, tempLeft = 0;
    int mid = (left + right) / 2;
    int tempArray[50000];
    
    
    //TRAVERSE ARRAY LEFT TO RIGHT
	if (left < right) {
		inv += p12(array, left, mid);       //*SORT LEFT SIDE
        inv += p12(array, mid + 1, right);  //*SORT RIGHT SIDE
        //printf("inv: %d\n", inv);

        //*MERGE
        i = left;
        j = 0;
        mid++;
	    tempMid = mid;
	    tempLeft = left;
		
        while((mid > tempLeft) && (tempMid <= right)) {
		    if (array[tempLeft] <= array[tempMid]) {
		    	tempArray[i] = array[tempLeft++];
		    }
		    else {
			    tempArray[i] = array[tempMid++];
			    inv += mid;
                inv -= tempLeft;
		    }
		    i++;
	    }

	    if(mid > tempLeft) {
            for (j = tempLeft; j <= mid; j++) 
			    tempArray[i++] = array[j];
	    }
	    else {
		    for (j = tempMid; j <= right; j++)
			    tempArray[i++] = array[j];
	    }

        //RESET ARRAY
	    for (j = left; j <= right; j++)
		    array[j] = tempArray[j];

        //printf("inv2: %d\n", inv);
	}
    return inv;
}

int p21(Point *p) {

    int i, j, k;
    double x1, x2, y1, y2, a, b, c;
    double tempC = 0;
    int hull1 = 0, hull2 = 0, hull3 = 0;
    int extPoint = 0;
    Point *convexHull = malloc(sizeof(Point) * MAXPOINTS);
    
    // for(i = 0; i < 100; i++) {
    //     convexHull[i].x = 0;
    //     convexHull[i].y = 0;
    // }

    //NESTED FOR LOOP TO TRAVERSE CONVEX HULL
    for(i = 0; i < MAXPOINTS; i++) {

        x1 = p[i].x;   //set x1
        y1 = p[i].y;   //set y1

        for(j = 0; j < MAXPOINTS; j++) {

            x2 = p[j].x;   //set x2
            y2 = p[j].y;   //set y2

            if(i != j) { //NOT SAME POINT

                a = y2 - y1;                //*CALC y2 - y1
                b = x1 - x2;                //*CALC x1 - x2
                c = (x1 * y2) - (y1 * x2);  //*CALC x1y2 - y1x2

                hull1 = hull2 = hull3 = 0;                
                for(k = 0; k < MAXPOINTS; k++) {    //*CHECK IF POINT ON CONVEX HULL
                    if(k != i && k != j) {  
                        tempC = ((a * p[k].x) + (b * p[k].y) - c);

                        if(tempC < 0.0) 
                            hull1 = 1;
                        else if(tempC > 0.0)
                            hull2 = 1;
                        else
                            hull3 = 1;

                        if(hull1 > 0 && hull2 > 0)
                            break;
                    }
                }

                if((hull1 == 1  && hull2 == 0) || (hull2 == 1 && hull1 == 0) ){
                    if(hull3 != 1) {
                        if(addPoint(convexHull, p[j].x, p[j].y))
                            extPoint++;
                        if(addPoint(convexHull, p[i].x, p[i].y))
                            extPoint++;
                    }
                }
            }
        }
    } 
    Point con[extPoint];
    for(int i = 0; i < extPoint; i++)            //copy array 
        con[i] = convexHull[i];

    qsort(con, extPoint, sizeof(Point), cmp);    //sort array of points
    printPoints(con, extPoint);                  //print array

    free(convexHull);

    return extPoint;
}

int addPoint(Point *p, double x, double y) {
    int i = 0;
    
    for(i = 0; i < 100; i++) {
        if(p[i].x == x && p[i].y == y){
            return 0;
        }
    }
    for(i = 0; i < 100; i++) {
        if(p[i].x == 0 && p[i].y == 0){
            p[i].x = x;
            p[i].y = y;
            break;
        }
    }

    return 1;
}

int p22(Point *p, int minX, int maxX) {

    Point *convexhull = malloc(sizeof(Point) * MAXPOINTS);

    int ext = 0;

    //call quick convex hull function on each side
    p22_quick(convexhull, p, p[minX], p[maxX], 1);  
    p22_quick(convexhull, p, p[minX], p[maxX], -1);

    ext = countPoints(convexhull);          //find number of extreme points
    Point con[ext];

    for(int i = 0; i < ext; i++)            //copy array 
        con[i] = convexhull[i];

    qsort(con, ext, sizeof(Point), cmp);    //sort array of points
    printPoints(con, ext);                  //print array

    free(convexhull);

    return ext;
}

void p22_quick(Point *h, Point *p, Point pLeft, Point pRight, int s) {
    
    int i = 0;
    int hull = -1;
    int temp = 0;
    double c = 0.0;
    int side = 0;
    int maxLength = 0;

    for(i = 0; i < MAXPOINTS; i++) {

        //*CALC LENGTH AND SIDE
        temp = abs((p[i].y - pLeft.y) * (pRight.x - pLeft.x) - (pRight.y - pLeft.y) * (p[i].x - pLeft.x));
        c = (p[i].y - pLeft.y) * (pRight.x - pLeft.x) - (pRight.y - pLeft.y) * (p[i].x - pLeft.x);

        //GET SIDE
        if( c < 0)
            side = -1;
        else if(c > 0)
            side = 1;
        else
            side = 0;
            
        if(side == s && temp > maxLength){ 
            hull = i;
            maxLength = temp;
        }
    }

    //*ADD POINT TO HULL
    if(hull == -1) {
        addPoint(h, pLeft.x, pLeft.y);
        addPoint(h, pRight.x, pRight.y);
        return;
    }


    c = (p[hull].y - pLeft.y) * (pRight.x - pLeft.x) - (pRight.y - pLeft.y) * (p[hull].x - pLeft.x);
    //GET SIDE
    if( c < 0)
        side = -1;
    else if(c > 0)
        side = 1;
    else
        side = 0;

    p22_quick(h, p, p[hull], pLeft, -side);     //*RECURSIVE CALL

    c = (p[hull].y - pRight.y) * (pLeft.x - pRight.x) - (pLeft.y - pRight.y) * (p[hull].x - pRight.x);
    //GET SIDE
    if( c < 0)
        side = -1;
    else if(c > 0)
        side = 1;
    else
        side = 0;

    p22_quick(h, p, p[hull], pRight, -side);    //*RECURSIVE CALL

}

int countPoints(Point *p) {

    int i = 0;
    int count = 0;

    for(i = 0; i < MAXPOINTS; i++) {
        count++;
        if(p[i].x == 0.0 && p[i].y == 0.0 && p[i + 1].x == 0.0 && p[i + 1].y == 0.0)
            break;
    }
    count--;
    return count;
}

void printPoints(Point p[], int n) {
    int i = 0;
    int count = 0;
    for(i = n - 1; i >= 0; i--) {
        count++;
        printf("%d) %.1f  \t%.1f\n", count, p[i].x, p[i].y);
    }
}

int cmp (const void * a, const void * b) {

    Point *pointA = (Point *)a;
    Point *pointB = (Point *)b;

    if (pointA->x > pointB->x)
        return -1;
    else if (pointA->x < pointB->x)
        return 1;
    else
        return 0;
}