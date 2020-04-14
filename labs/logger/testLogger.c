#include <stdio.h>

int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

int main(int argc, char const *argv[])
{
    infof("Hello World! INFO:\nchar: %c\nbase 10 int: %d\nbase 8 int: %o\nstring: %s\nbase 16 int: %x\nfloat: %f\npercentage sign: %%\n", 'a', -123, 10, "holis", 100, 64.64);
    infof("Format error: %m\n");

    warnf("Hello World! WARNING:\nchar: %c\nbase 10 int: %d\nbase 8 int: %o\nstring: %s\nbase 16 int: %x\nfloat: %f\npercentage sign: %%\n", 'a', -123, 10, "holis", 100, 64.64);
    warnf("Format error: %m\n");

    errorf("Hello World! ERROR:\nchar: %c\nbase 10 int: %d\nbase 8 int: %o\nstring: %s\nbase 16 int: %x\nfloat: %f\npercentage sign: %%\n", 'a', -123, 10, "holis", 100, 64.64);
    errorf("Format error: %m\n");

    panicf("Hello World! PANIC:\nchar: %c\nbase 10 int: %d\nbase 8 int: %o\nstring: %s\nbase 16 int: %x\nfloat: %f\npercentage sign: %%\n", 'a', -123, 10, "holis", 100, 64.64);
    panicf("Format error: %m\n");
    return 0;
}
