//c++ program
//design by >^.^<
//@date    2018-10-22 19:23:00
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>

void lsNoArgs(const char *dirName)
{
    DIR *dir;
    if(NULL == (dir = opendir(dirName)))
        perror("opendir");
    struct dirent *pdir;
    while(NULL != (pdir = readdir(dir)))
    {
        printf("%s\n", pdir->d_name);
    }
}

int main(int argc, char *argv[])
{
    if(1 == argc)
        lsNoArgs(".");
    else {
        lsNoArgs(argv[argc - 1]);
    }
}
