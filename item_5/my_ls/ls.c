//c++ program
//design by >^.^<
//@date    2018-10-22 20:06:51
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void doLs_l(const char *dirname);
void doLs(const char *dirname);
void fileInfo(const char *filename);

void doLs(const char *dirname) // 实现最简单的ls，仅仅罗列某个目录下的信息
{
    DIR *dirp = opendir(dirname);
    if(NULL == dirp )   
        perror("opendir");
    struct dirent * direntp;
    while(NULL != (direntp = readdir(dirp)))
    {
        printf("%s\t", direntp->d_name);
    }
    printf("\n");
}

void doLs_l(const char *dirname) // 实现ls -l 
{
    DIR *dirp = opendir(dirname);
    if(NULL == dirp )   
        perror("opendir");
    struct dirent * direntp;
    char jointPath[256] = {0};
    while(NULL != (direntp = readdir(dirp)))
    {
        // 当我们使用绝对路径时，如果不使用路径拼接，stat则会依然将其当做以当前路径开始进行处理
        // 路径拼接后，无论是使用相对路径还是绝对路径都OK
        sprintf(jointPath, "%s%s%s", dirname, "/", direntp->d_name); // 路径拼接
        fileInfo(jointPath);
    }
}

void processModestr(char *modestr, mode_t mode) // 处理权限位
{
    memset(modestr, '-', 10);
    if(S_ISREG(mode)) modestr[0] = '-';
    if(S_ISDIR(mode)) modestr[0] = 'd';
    if(S_ISCHR(mode)) modestr[0] = 'c';
    if(S_ISBLK(mode)) modestr[0] = 'b';
    if(S_ISFIFO(mode)) modestr[0] = 'p';
    if(S_ISLNK(mode)) modestr[0] = 'l';
    if(S_ISSOCK(mode)) modestr[0] = 's';

    if(mode & S_IRUSR) modestr[1] = 'r';
    if(mode & S_IWUSR) modestr[2] = 'w';
    if(mode & S_IXUSR) modestr[3] = 'x';
    if(mode & S_IRGRP) modestr[4] = 'r';
    if(mode & S_IWGRP) modestr[5] = 'w';
    if(mode & S_IXGRP) modestr[6] = 'x';
    if(mode & S_IROTH) modestr[7] = 'r';
    if(mode & S_IWOTH) modestr[8] = 'w';
    if(mode & S_IXOTH) modestr[9] = 'x';
    modestr[10] = 0;
}

char * uidToUsername(uid_t uid) // 根据uid获得用户名
{
    struct passwd *pwp;
    if(NULL == (pwp = getpwuid(uid)))
        perror("getpwuid");
    return pwp->pw_name;
}

char * gidToGroupname(gid_t gid) // 根据gid获取组名
{
    struct group *gpp;
    if(NULL == (gpp = getgrgid(gid)))
        perror("getpwuid");
    return gpp->gr_name;
}

void fileInfo(const char *filename) // 输出具体文件信息
{
    struct stat info;
    char buff[26] = {0};
    char modestr[11] = {0};
    if(-1 == stat(filename, &info))
        perror("stat");
    processModestr(modestr, info.st_mode);
    printf("%s", modestr); // 权限位
    printf("%6d", info.st_nlink); // 链接数
    printf("%8s", uidToUsername(info.st_uid));
    printf("%8s", gidToGroupname(info.st_gid));
    printf("%8lld", info.st_size); // 文件大小
    strcpy(buff, 8 + ctime(&info.st_mtime)); // 越过Www星期几，Mmm月份
    if(buff[strlen(buff) - 1] == '\n')
        buff[strlen(buff) - 1] = 0;
    printf("%s", buff); // ctime会自带\n需要处理
    printf("%8s\n", filename);
}

void processSwitch(char choose, const char *currDir)
{
    switch(choose)
    {
        case 'l':
            doLs_l(currDir);
            break;
        default: 
            printf("unimplented option\n");
            break;
    }
}
int main(int argc, char *argv[])
{
    if(1 == argc)
    {
        doLs(".");
    }
    else if(2 == argc)
    {
        if(argv[1][0] == '-') // 如果是选项
        {
            char choose = argv[1][1];
            const char *currDir = ".";
            processSwitch(choose, currDir);
        }else{
            doLs(argv[1]);
        }
    }
    else if(3 == argc && argv[1][0] == '-')
    {
        char choose = argv[1][1];
        const char *currDir = argv[2];
        processSwitch(choose, currDir);
    }
    else{
        printf("please use ./ls [-参数] [Path]\n");
    }
}
