#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday) {
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int leap = (year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0);
    int myYearDay = yearday;

    if((leap && yearday > 366) || (!leap && yearday > 365) || (yearday < 1)) {
        printf("Invalid year day!\n");
        return;
    }

    if(leap) {
        days[1] = 29;
    }

    for(int i = 0; i <12; i++) {
        if(myYearDay > days[i]) {
            myYearDay -= days[i];
        } else {
            *pmonth = i + 1;
            *pday = myYearDay;
            break;
        }
    }
}

/* month_name:  return name of n-th month */
char *month_name(int n) {

   static char *name[] = {"Illegal month", "Jan", "Feb", "Mar",
   "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

   return (n < 1 || n > 12) ? name[0] : name[n];
}

int main(int argc, char *argv[])
{
    if(argc == 3) {
        int year = atoi(argv[1]);
        int yearday = atoi(argv[2]);
        int x = 0, y = 0;
        int *month = &x, *monthD = &y;

        month_day(year, yearday, month, monthD);

        char *monthS = month_name(*month);
        char dayS[2];
        sprintf(dayS, "%d", *monthD);
        if(*monthD < 10) {
            sprintf(dayS, "0%d", *monthD);
        }

        printf("%s %s, %d\n", monthS, dayS, year);
    } else {
        printf("Invalid number of arguments!\n");
    }
    return 0;
}
