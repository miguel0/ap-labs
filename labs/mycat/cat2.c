#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int myRead(int file);
void myWrite(int c, int file);
void myWriteStr(char *str, int file);

/* filecopy:  copy file ifp to file ofp */
void filecopy(int ifp, int ofp)
{
    int c;

    while ((c = myRead(ifp)) != EOF)
        myWrite(c, ofp);
}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{
    int fp;
    void filecopy(int ifp, int ofp);
    char *prog = argv[0];   /* program name for errors */

    if (argc == 1)  /* no args; copy standard input */
        filecopy(fileno(stdin), fileno(stdout));
    else
        while (--argc > 0)
            if ((fp = open(*++argv, O_RDONLY, 0600)) < 0) {
                myWriteStr(prog, fileno(stderr));
                myWriteStr(": can′t open ", fileno(stderr));
                myWriteStr(*argv, fileno(stderr));
                myWriteStr("\n", fileno(stderr));
                // fprintf(stderr, "%s: can′t open %s\n", prog, *argv);
                return 1;
            } else {
                filecopy(fp, fileno(stdout));
                close(fp);
            }

    if (ferror(stdout)) {
        myWriteStr(prog, fileno(stderr));
        myWriteStr(": error writing stdout\n", fileno(stderr));
        // fprintf(stderr, "%s: error writing stdout\n", prog);
        return 2;
    }

    return 0;
}

int myRead(int file)
{
    int buffer[1];

    if(read(file, buffer, 1) == 0) {
        return EOF;
    }

    return buffer[0];
}

void myWrite(int c, int file)
{
    write(file, &c, 1);
}

void myWriteStr(char *str, int file)
{
    for(int i = 0; i < strlen(str); i++) {
        myWrite((int) str[i], file);
    }
}
