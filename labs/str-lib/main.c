#include <stdio.h>

int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

int main(int argc, char *argv[])
{
    if(argc < 4) {
        printf("Invalid number of arguments.");
        return 1;
    }

    char *concat = mystradd(argv[1], argv[2]);
    int find = mystrfind(concat, argv[3]);

    printf("Initial Lenght      : %d\n", mystrlen(argv[1]));
    printf("New String          : %s\n", concat);
    if(find == -1) {
        printf("SubString was found : no\n");
    } else {
        printf("SubString was found : yes, on index %d\n", find);
    }
    return 0;
}
