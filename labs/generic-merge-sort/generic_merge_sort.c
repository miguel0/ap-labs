#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 100

void getInput(char* string) {
    int i, c;
    i = 0;

    while((c = getchar()) != '\n') {
        string[i] = c;
        i++;
    }
    string[i] = '\0';
}

int numcmp(char *s1, char *s2)
{
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);
    if(v1 < v2) {
        return -1;
    } else if(v1 > v2) {
        return 1;
    } else {
        return 0;
    }
}

void merge(char arr[][MAXLEN], int l, int m, int r, int(*comp)(void *, void *))
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    char L[n1][MAXLEN], R[n2][MAXLEN]; 
  
    for (i = 0; i < n1; i++) 
        strcpy(L[i], arr[l + i]);
    for (j = 0; j < n2; j++) 
        strcpy(R[j], arr[m + 1+ j]);
  
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) 
    { 
        if ((*comp)(L[i], R[j]) <= 0) 
        { 
            strcpy(arr[k], L[i]);
            i++; 
        } 
        else
        { 
            strcpy(arr[k], R[j]);
            j++; 
        } 
        k++; 
    }

    while (i < n1) 
    { 
        strcpy(arr[k], L[i]); 
        i++; 
        k++; 
    }

    while (j < n2) 
    { 
        strcpy(arr[k], R[j]); 
        j++; 
        k++; 
    } 
} 

void mergeSort(char *arr[MAXLEN], int l, int r, int(*comp)(void *, void *)) 
{ 
    if (l < r)
    {
        int m = l+(r-l)/2;
  
        mergeSort(arr, l, m, comp); 
        mergeSort(arr, m+1, r, comp); 
  
        merge(arr, l, m, r, comp);
    } 
}

int main(int argc, char const *argv[])
{
    // Find out if it's sorting strings or ints
    int numeric = 0;
    if(argc > 1 && strcmp(argv[1], "-n") == 0) {
        numeric = 1;
    }

    // Get input for the array size
	int i ,n;
	printf("Enter array length: ");
	scanf("%d",&n);
    getchar();

    if(n < 1) {
        printf("Enter a number greater than 0.\n");
        return 1;
    }

    char strings[n][MAXLEN];
	
    // Assign values to the array
	for(i = 0; i < n; i++)
	{
		printf("%s[%d] ? ", numeric ? "Number" : "String", i );
		getInput(strings[i]);
	}
    
    // Print unsorted array
    printf("Unsorted array: {");
    for(i  = 0; i < n - 1; i++) {
        printf("%s, ", strings[i]);
    }
    printf("%s}\n", strings[n - 1]);

    // Sort
    mergeSort(strings, 0, n - 1, (int (*)(void *, void *))(numeric ? numcmp : strcmp));

    // Print sorted array
    printf("Sorted array: {");
    for(i  = 0; i < n - 1; i++) {
        printf("%s, ", strings[i]);
    }
    printf("%s}\n", strings[n - 1]);

    return 0;
}
