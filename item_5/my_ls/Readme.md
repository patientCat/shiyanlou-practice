# ls命令实现
## ls命令介绍
ls用来显示目标列表，一般使用的方式为
`ls -[参数] [Path]`
如果不加Path的话，则是默认显示当前文件夹下

常见参数如下
> -l：以长格式显示目录下的内容列表。输出的信息从左到右依次包括文件名，文件类型、权限模式、硬连接数、所有者、组、文件大小和文件的最后修改时间等;
-a：显示所有档案及目录；
-r：以文件名反序排列并输出目录内容列表；
-t：用文件和目录的更改时间排序；
-R：递归处理，将指定目录下的所有文件及子目录一并处理；
-r：以文件名反序排列并输出目录内容列表；

常用参数组合
> ls -rtl ：非常实用，反向按时间排序，通常我们需要查看一个目录下面最近修改的文件，在面临很多文件的时候，可以用这条命令很快的将最近修改的文件列在前面几行
ls --full-time：非常实用，在写一些shell脚本的时候需要获取文件被更新的时间，可以用ls的列出完整的日期与时间，获取时间细微的变化

## 读取目录下所有文件
`DIR *opendir(const char *name);`
`struct dirent *readdir(DIR *dirp);`

第一个函数功能：根据name获得一个DIR流，DIR流类似FILE流，用于接下来的操作，如果认真研究的话，其本质就是一个结构体。<br/>
这里主要对`const char *name`参数的理解，如果不加"/"，比如"."或者"testdir"，此时为相对路径，否则为绝对路径。<br/>
因此如果配合stat这个函数的时候，必然需要路径拼接（这里需要思考一下）<br/>

第二个函数功能：读取DIR流获得目录信息，存储在<br/>
```c
struct dirent {
       ino_t          d_ino;         /* inode number */
       off_t          d_off;         /* offset to the next dirent */
       unsigned short d_reclen;      /* length of this record */
       unsigned char  d_type;        /* type of file; not supportedby all file system types */
       char           d_name[256];   /* filename */
};
```
我们需要的是d_name，即目录下，各个文件的名字。注意这里的名字仅仅是相对名字。<br/>
`readdir函数每成功读取一次，会自动偏移到目录流中读取下一目录条目。`因此，每次调用完readdir，要将其返回的信息立刻存储或者输出，否则
会被下次函数发生覆盖<br/>


## 获取文件信息
### stat
https://www.cnblogs.com/sylar5/p/6491033.html

```c
int stat(const char *pathname, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *pathname, struct stat *buf);
```

1. pathname：对于这个参数来说，如果我们使用"/usr/include"则代表绝对路径。如果我们使用"include"则使用相对路径，会默认在当前路径下搜索
2. buf：为传出参数，是我们必须提供给内核的
```c
struct stat {
dev_t st_dev; /*如果是设备，返回设备表述符，否则为 0*/ 
ino_t st_ino; /* i 节点号 */   唯一标识
mode_t st_mode; /* 文件类型 */  16位，高4位表示文件类型，低9位表示权限
nlink_t st_nlink; /* 链接数 */ 引用计数，硬链接
uid_t st_uid; /* 属主 ID */   通过getpwuid可以转换成用户名
gid_t st_gid; /* 组 ID */    通过getgrgid可以转换成组名
dev_t st_rdev; /* 设备类型*/ 
off_t st_size; /* 文件大小，字节表示 */ 
blksize_t st_blksize; /* 块大小*/ 
blkcnt_t st_blocks; /* 块数 */ 
time_t st_atime; /* 最后访问时间*/ 
time_t st_mtime; /* 最后修改时间*/ 通过ctime可以转换成人类可读时间
time_t st_ctime; /* 最后权限修改时间 */ 
};
```
我们一般最主要关注的是文件类型和文件大小。首先就目录文件而言，就比较重要。

早期使用方式，总是需要将st_mode先与屏蔽字然后与名为S_IFxxx常量相比较
```c
struct stat buf;
if(-1 == stat("test.txt", &buf))
{
    perror("stat error");
}
if( (buf.st_mode & S_IFMT) == S_IFREG)
    printf("this is a common file\n");

if(-1 == stat("testdir", &buf))
{
    perror("stat error");
}
if( (buf.st_mode & S_IFMT) == S_IFDIR)
    printf("this is a dir\n");
```
后期提供了宏使用，例如
```c
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
```


当然最直接的方式就是直接使用数字去判断，因为st_mode中高4位为文件类型，低9位为ugo的rwx权限
> st_mode解释<br/>
> 它是一个 16 位的二进制数，前 4 位用作文件类型，1 代表具有某个属性，0 代表没有；<br/>
> 接下来的3位分为 user-ID，set-group-ID 位和 sticky 位，最后9位是许可权限，分三组，每组 3 位；<br/>
> 这也是为什么S_IFMT = 0170000，其中均为8进制，每个8进制我们知道可以化为3个二进制位，因此1 + 3 * 5 = 16位

### 关于路径拼接
目录树如下，我们的ls程序在/home/jmx/ls下，当前目录也在这个文件夹下，现在使用./ls -l /usr/local/include
```
/usr/local/include
        |---- abc.txt
        |---- testdir


/home/jmx/ls/
        |---- ls
```
假设我们使用绝对路径`/usr/local/include`传给opendir , 此时通过readdir获得俩个文件名 abc.txt 和 testdir，如果我们直接传给
stat，那么此时stat首先会在/home/jmx/ls/也就是当前文件夹下找这几个文件名，当然找不到。自然会报错误。<br/>
使用路径拼接后，传入stat的文件名为/usr/local/include/abc.txt 和/usr/local/include/testdir，因为使绝对路径，自然不会出错。<br/>

