#include<stdio.h>
#include<stdarg.h>
#include <stdlib.h>

#define RESET		0
#define BRIGHT 		1
#define DIM			2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED			1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define WHITE		7

char *convert(unsigned int num, int base)
{
	static char *representation= "0123456789ABCDEF";
	static char buffer[50];
	char *ptr;

	ptr = &buffer[49];
	*ptr = '\0';
	
	do
	{
		*--ptr = representation[num%base];
		num /= base;
	} while(num != 0);
	
	return ptr;
}

void textcolor(int attr, int fg, int bg)
{
	char command[13];

	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

int infof(char* format, ...)
{
	char *traverse, *s;
	char buffer[1000];
	int i;
	double f;
	
	va_list arg;
	va_start(arg, format);

	textcolor(BRIGHT, BLUE, BLACK);
	
	for(traverse = format; *traverse != '\0'; traverse++)
	{
		while(*traverse != '%')
		{
			if(*traverse == '\0')
			{
				va_end(arg);
    			return 4;
			}

			putchar(*traverse);
			traverse++;
		}
		
		traverse++;
		
		switch(*traverse)
		{
			case 'c' : i = va_arg(arg, int);
						putchar(i);
						break;
			case 'd' : i = va_arg(arg, int);
						if(i<0)
						{
							i = -i;
							putchar('-');
						}
						fputs(convert(i, 10), stdout);
						break;
			case 'o': i = va_arg(arg, unsigned int);
						fputs(convert(i, 8), stdout);
						break;
			case 's': s = va_arg(arg, char *);
						fputs(s, stdout);
						break;
			case 'x': i = va_arg(arg, unsigned int);
						fputs(convert(i, 16), stdout);
						break;
			case 'f': f = va_arg(arg, double);
						fputs(gcvt(f, 10, buffer), stdout);
						break;
			case '%': putchar('%');
						break;
			default: printf("Invalid formatting!\n");
						va_end(arg);
    					return 4;
		}
	}

	va_end(arg);
	textcolor(RESET, WHITE, BLACK);
    return 4;
}

int warnf(char* format, ...)
{
	char *traverse, *s;
	char buffer[1000];
	int i;
	double f;
	
	va_list arg;
	va_start(arg, format);

	textcolor(BRIGHT, YELLOW, BLACK);
	
	for(traverse = format; *traverse != '\0'; traverse++)
	{
		while(*traverse != '%')
		{
			if(*traverse == '\0')
			{
				va_end(arg);
    			return 5;
			}

			putchar(*traverse);
			traverse++;
		}
		
		traverse++;
		
		switch(*traverse)
		{
			case 'c' : i = va_arg(arg, int);
						putchar(i);
						break;
			case 'd' : i = va_arg(arg, int);
						if(i<0)
						{
							i = -i;
							putchar('-');
						}
						fputs(convert(i, 10), stdout);
						break;
			case 'o': i = va_arg(arg, unsigned int);
						fputs(convert(i, 8), stdout);
						break;
			case 's': s = va_arg(arg, char *);
						fputs(s, stdout);
						break;
			case 'x': i = va_arg(arg, unsigned int);
						fputs(convert(i, 16), stdout);
						break;
			case 'f': f = va_arg(arg, double);
						fputs(gcvt(f, 10, buffer), stdout);
						break;
			case '%': putchar('%');
						break;
			default: printf("Invalid formatting!\n");
						va_end(arg);
    					return 5;
		}
	}

	va_end(arg);
	textcolor(RESET, WHITE, BLACK);
    return 5;
}

int errorf(char* format, ...)
{
	char *traverse, *s;
	char buffer[1000];
	int i;
	double f;
	
	va_list arg;
	va_start(arg, format);

	textcolor(BRIGHT, RED, BLACK);
	
	for(traverse = format; *traverse != '\0'; traverse++)
	{
		while(*traverse != '%')
		{
			if(*traverse == '\0')
			{
				va_end(arg);
    			return 6;
			}

			putchar(*traverse);
			traverse++;
		}
		
		traverse++;
		
		switch(*traverse)
		{
			case 'c' : i = va_arg(arg, int);
						putchar(i);
						break;
			case 'd' : i = va_arg(arg, int);
						if(i<0)
						{
							i = -i;
							putchar('-');
						}
						fputs(convert(i, 10), stdout);
						break;
			case 'o': i = va_arg(arg, unsigned int);
						fputs(convert(i, 8), stdout);
						break;
			case 's': s = va_arg(arg, char *);
						fputs(s, stdout);
						break;
			case 'x': i = va_arg(arg, unsigned int);
						fputs(convert(i, 16), stdout);
						break;
			case 'f': f = va_arg(arg, double);
						fputs(gcvt(f, 10, buffer), stdout);
						break;
			case '%': putchar('%');
						break;
			default: printf("Invalid formatting!\n");
						va_end(arg);
    					return 6;
		}
	}

	va_end(arg);
	textcolor(RESET, WHITE, BLACK);
    return 6;
}

int panicf(char* format, ...)
{
	char *traverse, *s;
	char buffer[1000];
	int i;
	double f;
	
	va_list arg;
	va_start(arg, format);

	textcolor(BRIGHT, WHITE, BLACK);
	
	for(traverse = format; *traverse != '\0'; traverse++)
	{
		while(*traverse != '%')
		{
			if(*traverse == '\0')
			{
				va_end(arg);
    			return 100;
			}

			putchar(*traverse);
			traverse++;
		}
		
		traverse++;
		
		switch(*traverse)
		{
			case 'c' : i = va_arg(arg, int);
						putchar(i);
						break;
			case 'd' : i = va_arg(arg, int);
						if(i<0)
						{
							i = -i;
							putchar('-');
						}
						fputs(convert(i, 10), stdout);
						break;
			case 'o': i = va_arg(arg, unsigned int);
						fputs(convert(i, 8), stdout);
						break;
			case 's': s = va_arg(arg, char *);
						fputs(s, stdout);
						break;
			case 'x': i = va_arg(arg, unsigned int);
						fputs(convert(i, 16), stdout);
						break;
			case 'f': f = va_arg(arg, double);
						fputs(gcvt(f, 10, buffer), stdout);
						break;
			case '%': putchar('%');
						break;
			default: printf("Invalid formatting!\n");
						va_end(arg);
    					return 100;
		}
	}

	va_end(arg);
	textcolor(RESET, WHITE, BLACK);
    return 100;
}
