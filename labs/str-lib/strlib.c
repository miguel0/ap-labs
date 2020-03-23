#include <stdlib.h>

int mystrlen(char *str){
    int c, i;

    i = 0;

    while((c = str[i]) != '\0') {
        i++;
    }    

    return i;
}

char *mystradd(char *origin, char *addition){
    int l1, l2, i;

    l1 = mystrlen(origin);
    l2 = mystrlen(addition);

    char *res = malloc(sizeof(char) * (l1 + l2 + 1));

    for(i = 0; i < l1; i++) {
        res[i] = origin[i];
    }
    for(i = 0; i < l2; i++) {
        res[i + l1] = addition[i];
    } 
    res[l1 + l2] = '\0';

    return res;
}

int mystrfind(char *origin, char *substr){
    int l1, l2, sameChar, i, j;

    l1 = mystrlen(origin);
    l2 = mystrlen(substr);

    for(i = 0; i < l1 - l2 + 1; i++) {
        sameChar = 0;
        for(j = i; j < l2 + i; j++) {
            if(origin[j] == substr[j - i]) {
                sameChar++;
            } else {
                break;
            }

            if(sameChar == l2) {
                return i;
            }
        }
    }

    return -1;
}
