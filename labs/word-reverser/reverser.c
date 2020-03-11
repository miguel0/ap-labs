#include <stdio.h>

#define MAX_SIZE 10000

void reverse(int len, char *word) {
    int i, tmp;

    for(i = 0; i < len/2; i ++) {
        tmp = word[i];
        word[i] = word[len - i - 1];
        word[len - i - 1] = tmp;
    }

    printf("%s\n", word);
}

int main(){
    char word[MAX_SIZE];
    int i, c;
    i = 0;

    while ((c = getchar()) != EOF) {
        if(c == '\n') {
            word[i] = '\0';
            reverse(i, word);
            i = 0;
        } else {
            word[i] = c;
            i++;
        }
    }
    
    return 0;
}
