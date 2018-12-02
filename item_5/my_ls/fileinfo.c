//c++ program
//design by >^.^<
//@date    2018-10-22 19:37:12
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void showFileinfo(struct stat *info)
{
    printf("mode: %o\n", info->st_mode);
    printf("links: %ld\n", info->st_nlink);
    printf("uid: %d\n", info->st_uid);
    printf("gid: %d\n", info->st_gid);
    printf("size: %ld\n", info->st_size);
    printf("atime: %ld\n", info->st_atime);
    printf("mtime: %ld\n", info->st_mtime);
}

int main(int argc, char *argv[])
{
    struct stat info;

    if(argc > 1)
    {
        if(-1 != stat(argv[1], &info))
        {
            showFileinfo(&info);
        }else{
            perror("stat");
        }
    }
    else{
        printf("please use ./a.out filename\n");
    }
    return 0;
}
