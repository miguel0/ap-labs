#include <stdio.h>

/* print Fahrenheit-Celsius table */

int main(int argc, char *argv[])
{
    if(argc == 2) {
        printf("Fahrenheit: %3d, Celcius: %6.1f\n", atoi(argv[1]), (5.0/9.0)*(atoi(argv[1])-32));
    } else if(argc == 4) {
        int fahr;

        for (fahr = atoi(argv[1]); fahr <= atoi(argv[2]); fahr = fahr + atoi(argv[3]))
	        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    } else {
        printf("Wrong number of arguments!");
    }

    return 0;
}
