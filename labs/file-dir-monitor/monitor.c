#define _XOPEN_SOURCE 500

#include <sys/inotify.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#include <ftw.h>

#include "logger.h"

#define MAX_LEN 1024
#define MAX_EVENTS 1024
#define LEN_NAME 16
#define EVENT_SIZE (sizeof (struct inotify_event))
#define BUF_LEN (MAX_EVENTS * (EVENT_SIZE + LEN_NAME))

int fd;

int add_dir(const char *fpath, const struct stat *sb, int typeflag)
{
    if(typeflag == FTW_D) {
        inotify_add_watch(fd, fpath, IN_CREATE | IN_MOVED_FROM | IN_DELETE);
    }
    return 0;
}

int main(int argc, char **argv)
{
    int length, i = 0;
    char buffer[BUF_LEN], root[MAX_LEN];

    switch(argc) {
        case 1:
            infof("No directory specified. Will monitor the entire filesystem...\n\n");
            strcpy(root,"/");
            break;
            
        case 2:
            strcpy(root,argv[1]);
            if(root[strlen(root)-1]!='/')
                strcat(root,"/");
            puts(root);
            break;
            
        default:
            infof("Ignoring all other arguments after the first\n");
    }

    fd = inotify_init();
    if (fd < 0) {
        errorf( "Couldn't initialize inotify");
        exit(1);
    }

    ftw(root, add_dir, 64);

    while(1)
    {
        i = 0;
        length = read(fd, buffer, BUF_LEN);

        if (length < 0) {
            errorf("Read error.\n");
            exit(1);
        }  

        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) buffer;
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    if (event->mask & IN_ISDIR) {
                        infof("DIR: %s CREATED\n", event->name);
                        ftw(root, add_dir, 64);
                    }
                    else
                        infof("FILE: %s CREATED\n", event->name);
                }
                
                if (event->mask & IN_MOVED_FROM) {
                    if (event->mask & IN_ISDIR)
                        infof("DIR: %s MODIFIED\n", event->name);
                    else
                        infof("FILE: %s MODIFIED\n", event->name);
                }
                
                if (event->mask & IN_DELETE) {
                    if (event->mask & IN_ISDIR)
                        infof("DIR: %s DELETED\n", event->name);
                    else
                        infof("FILE: %s DELETED\n", event->name);
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    (void) close(fd);

    return 0;
}