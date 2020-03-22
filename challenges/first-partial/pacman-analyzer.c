#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define REPORT_FILE "packages_report.txt"
#define ARR_LEN 10000
#define MAX_LEN 1000

typedef struct Package Package;

void analizeLog(char *logFile, char *report);
void update(Package *arr, int i, char *newDate);
void subStr(char *str, int s, int e, char *out);
int searchIndex(Package *arr, int len, char *name);
void getPackageName(char *line, int start, char *out);
void initializeArray(Package *arr);
void myWrite(char *toWrite, int file);

struct Package
{
    char name[50];
    char installDate[20];
    char lastUpdate[20];
    char removalDate[20];
    int updateNum;
};

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage:./pacman-analizer.o pacman.log\n");
        return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int file;
    file = open(logFile, O_RDONLY, 0600);

    if(file < 0) {
        printf("Unable to open file.\n");
        exit(1);
    }

    Package arr[ARR_LEN];
    char readBuffer[1], state1[12], state2[12], state3[12], date[20], line[MAX_LEN], myStr[MAX_LEN], name[MAX_LEN];
    int installed, removed, upgraded, i, len, bufferSize, arrIndex, linesRead;

    initializeArray(arr);
    installed = removed = upgraded = i = len = bufferSize = 0;

    for(;;) {
        linesRead = read(file, readBuffer, 1);
        
        if(linesRead < 0) {
            printf("Unable to read file.\n");
            exit(1);
        }
        if (linesRead == 0) {
            break;
        }

        if(readBuffer[0] != '\n') {
            line[i] = readBuffer[0];
            i++;
        } else {
            line[i] = '\0';

            subStr(line, 26, 35, state1);
            subStr(line, 26, 34, state2);
            subStr(line, 26, 33, state3);
            
            if(!strcmp(state1, "installed") && i > 33) {
                getPackageName(line, 36, name);
                arrIndex = searchIndex(arr, len, name);
                subStr(line, 1, 17, date);

                if(arrIndex >= 0) {
                    strcpy(arr[arrIndex].installDate, date);
                    strcpy(arr[arrIndex].removalDate, "");

                    installed++;
                } else {
                    strcpy(arr[len].name, name);
                    strcpy(arr[len].installDate, date);
                    
                    len++;
                    installed++;
                }
            } else if(!strcmp(state2, "upgraded") && i > 32) {
                getPackageName(line, 35, name);
                arrIndex = searchIndex(arr, len, name);
                subStr(line, 1, 17, date);

                update(arr, arrIndex, date);

                upgraded++;
            } else if(!strcmp(state3, "removed") && i > 31) {
                getPackageName(line, 34, name);
                arrIndex = searchIndex(arr, len, name);
                subStr(line, 1, 17, date);

                strcpy(arr[arrIndex].removalDate, date);

                removed++;
            }

            i = 0;
        }
    }

    if(close(file) < 0) {
        printf("Unable to close file.\n");
        exit(1);
    }

    remove(report);

    file = open(report, O_WRONLY | O_APPEND | O_CREAT, 0644);

    if(file < 0) {
        printf("Unable to open file.\n");
        exit(1);
    }

    myWrite("Pacman Packages Report\n----------------------\n", file);
    sprintf(myStr, "- Installed packages : %d\n", installed);
    myWrite(myStr, file);
    sprintf(myStr, "- Removed packages   : %d\n", removed);
    myWrite(myStr, file);
    sprintf(myStr, "- Upgraded packages  : %d\n", upgraded);
    myWrite(myStr, file);
    sprintf(myStr, "- Current installed  : %d\n", installed - removed);
    myWrite(myStr, file);
    sprintf(myStr, "\nList of packages\n----------------\n");
    myWrite(myStr, file);

    for(i = 0; i < len; i++) {
        sprintf(myStr, "- Package Name        : %s\n", strcmp(arr[i].name, "")? arr[i].name: "-");
        myWrite(myStr, file);

        sprintf(myStr, "  - Install date      : %s\n", strcmp(arr[i].installDate, "")? arr[i].installDate: "-");
        myWrite(myStr, file);

        sprintf(myStr, "  - Last update date  : %s\n", strcmp(arr[i].lastUpdate, "")? arr[i].lastUpdate: "-");
        myWrite(myStr, file);

        sprintf(myStr, "  - How many updates  : %d\n", arr[i].updateNum);
        myWrite(myStr, file);

        sprintf(myStr, "  - Removal date      : %s\n", strcmp(arr[i].removalDate, "")? arr[i].removalDate: "-");
        myWrite(myStr, file);
    }

    if(close(file) < 0) {
        printf("Unable to close file.\n");
        exit(1);
    }

    printf("Report is generated at: [%s]\n", report);
}

void update(Package *arr, int i, char *newDate) {
    arr[i].updateNum++;

    if(strlen(arr[i].lastUpdate) == 0) {
        strcpy(arr[i].lastUpdate, newDate);
    } else {
        char curStr[6], newStr[6];
        int cur, new;

        // Comprare years
        subStr(arr[i].lastUpdate, 0, 4, curStr);
        subStr(newDate, 0, 4, newStr);

        cur = atoi(curStr);
        new = atoi(newStr);

        if(new > cur) {
            strcpy(arr[i].lastUpdate, newDate);
            return;
        }

        // Comprare month
        subStr(arr[i].lastUpdate, 5, 7, curStr);
        subStr(newDate, 5, 7, newStr);

        cur = atoi(curStr);
        new = atoi(newStr);

        if(new > cur) {
            strcpy(arr[i].lastUpdate, newDate);
            return;
        }

        // Comprare day
        subStr(arr[i].lastUpdate, 8, 10, curStr);
        subStr(newDate, 8, 10, newStr);

        cur = atoi(curStr);
        new = atoi(newStr);

        if(new > cur) {
            strcpy(arr[i].lastUpdate, newDate);
            return;
        }

        // Comprare hour
        subStr(arr[i].lastUpdate, 11, 13, curStr);
        subStr(newDate, 11, 13, newStr);

        cur = atoi(curStr);
        new = atoi(newStr);

        if(new > cur) {
            strcpy(arr[i].lastUpdate, newDate);
            return;
        }

        // Comprare minute
        subStr(arr[i].lastUpdate, 14, 16, curStr);
        subStr(newDate, 14, 16, newStr);

        cur = atoi(curStr);
        new = atoi(newStr);

        if(new > cur) {
            strcpy(arr[i].lastUpdate, newDate);
            return;
        }
    }
}

void subStr(char *str, int s, int e, char *out) {
    for(int i = s; i < e; i++) {
        out[i - s] = str[i];
    }
    out[e - s] = '\0';
}

int searchIndex(Package *arr, int len, char *name) {
    for(int i = 0; i < len; i++){
        if(!strcmp(arr[i].name, name)) {
            return i;
        }
    }

    return -1;
}

void getPackageName(char *line, int start, char *out) {
    int i = 0;
    while(line[i + start] != ' ') {
        out[i] = line[i + start];
        i++;
    }
    out[i] = '\0';
}

void initializeArray(Package *arr) {
    for(int i = 0; i < ARR_LEN; i++) {
        Package temp;
        arr[i] = temp;

        strcpy(arr[i].name, "");
        strcpy(arr[i].installDate, "");
        strcpy(arr[i].lastUpdate, "");
        strcpy(arr[i].removalDate, "");
        arr[i].updateNum = 0;
    }
}

void myWrite(char *toWrite, int file) {
    char writeBuffer[1];
    int i, err;
    
    i = 0;

    while(toWrite[i] != '\0') {
        writeBuffer[0] = toWrite[i];

        err = write(file, writeBuffer, 1);
        if(err < 0) {
            printf("Unable to write file.\n");
            exit(1);
        }

        err = lseek(file, 1, SEEK_SET);
        if(err < 0) {
            printf("Unable to seek file.\n");
            exit(1);
        }

        i++;
    }
}