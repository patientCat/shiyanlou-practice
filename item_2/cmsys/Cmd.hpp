//c++ program
//design by >^.^<
//@date    2018-10-14 22:05:22

#ifndef _CMD_H_
#define _CMD_H_

enum CourseCmd
{
    // 打印命令帮助信息
    Cmd_PrintHelp = 0,
    // 退出程序
    Cmd_Exit = 1,
    // 打印课程信息
    Cmd_PrintCourse = 2,
    // 打印课程数量
    Cmd_PrintCourseNum = 3,
    // 打印最长的课程名称
    Cmd_PrintLongName = 4,
    // 删除最后一个课程
    Cmd_RemoveLastCourse = 5,
};
#endif /* Cmd_h */
