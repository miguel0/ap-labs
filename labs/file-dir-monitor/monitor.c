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

void add_watches(int fd, char *root)
{
    int wd;
    char *abs_dir;
    struct dirent *entry;
    DIR *dp;

    dp = opendir(root);
    if (dp == NULL) {
        errorf("Error opening the starting directory\n");
        exit(0);
    }

    wd = inotify_add_watch(fd, root, IN_CREATE | IN_MOVED_FROM | IN_DELETE);
    if (wd == -1) {
        errorf("Couldn't add watch to %s\n",root);
    }
    else {
        infof("Watching:: %s\n",root);
    }

    abs_dir = (char *)malloc(MAX_LEN);
    while((entry = readdir(dp))) {
        if (entry->d_type == 4)
        {
            strcpy(abs_dir,root);
            strcat(abs_dir,entry->d_name);
            
            wd = inotify_add_watch(fd, abs_dir, IN_CREATE | IN_MOVED_FROM | IN_DELETE);
            if (wd == -1)
                infof("Couldn't add watch to the directory %s\n",abs_dir);
            else
                infof("Watching:: %s\n",abs_dir);
        }
    }

    closedir(dp);
    free(abs_dir);
}

int add_dir(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    return inotify_add_watch(fd, fpath, IN_CREATE | IN_MOVED_FROM | IN_DELETE);
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

    add_watches(fd, root);

    while(1)
    {
        i = 0;
        length = read(fd, buffer, BUF_LEN);  

        if (length < 0) {
            errorf("read\n");
            exit(1);
        }  

        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    if (event->mask & IN_ISDIR) {
                        infof("DIR: %s CREATED\n", event->name);
                        if (nftw(root, add_dir, 64, FTW_PHYS) == -1) {
                            errorf("Couldn't add watch to %s\n", event->name);
                            exit(1);
                        }
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

                i += EVENT_SIZE + event->len;
            }
        }
    }

    (void) close(fd);

    return 0;
}