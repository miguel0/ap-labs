#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc > 1) {
        char *string = argv[1];
        char starting = 1;
        int counter[256];
        int maxLen = 1;
        int curLen = 0;
        int inStart = 0;
        int inCur = 0;
        int i;
        
        while(inStart < strlen(string)) {
            if(starting) {
                for(i = 0; i < 256; i++) {
                    counter[i] = 0;
                }
                curLen = 0;
                starting = 0;
                inCur = inStart;
            }

            if(counter[(int) string[inCur]] == 0) {
                counter[(int) string[inCur]]++;
                curLen++;
                if(curLen > maxLen) {
                    maxLen = curLen;
                }
                inCur++;
                if(inCur >= strlen(string)) {
                    break;
                }
            } else {
                starting = 1;
                inStart++;
            }
        }

        printf("%d", maxLen);
    }
    return 0;
}
