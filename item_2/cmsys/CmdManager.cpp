//c++ program
//design by >^.^<
//@date    2018-10-14 22:23:10
#include "CmdManager.hpp" 
#include <iostream> 

using std::cout; 
using std::endl; 

void 
CmdManager::init()
{
    // 初始化课程列表
    _manager.addCourse("Linux");
    _manager.addCourse("C");
    _manager.addCourse("C++");
    _manager.addCourse("Java");
    _manager.addCourse("Phython");
    // 初始化命令列表
    _cmdMap.insert(std::make_pair<CourseCmd, string>(Cmd_PrintHelp, "Help Info"));
    _cmdMap.insert(std::make_pair<CourseCmd, string>(Cmd_Exit, "Exit"));
    _cmdMap.insert(std::make_pair<CourseCmd, string>(Cmd_PrintCourse, "Print Course Info"));
    _cmdMap.insert(std::make_pair<CourseCmd, string>(Cmd_PrintCourseNum, "Print Course Num"));
    _cmdMap.insert(std::make_pair<CourseCmd, string>(Cmd_PrintLongName, "Print LongCourse Name"));
    _cmdMap.insert(std::make_pair<CourseCmd, string>(Cmd_RemoveLastCourse, "Remove Last Course"));
} 

void 
CmdManager::printAllHelp() const
{
    for(auto &a : _cmdMap)
    {
        cout << "Cmd_Id: " << a.first << "   " << "Cmd: " << a.second << endl;
    }
}

void 
CmdManager::printCmdHelp(const CourseCmd cmd) const
{
    auto ret = _cmdMap.find(cmd);
    cout << "Cmd: " << ret->second  << endl;
}

bool 
CmdManager::handleCmd(const CourseCmd cmd)
{
    // 确认命令我们是否支持
    auto num = _cmdMap.count(cmd);
    if(!num)
    {
        cout << "Not Found" << endl;
        return true;
    }
    switch(cmd)
    {
        case Cmd_PrintHelp: // 打印帮助信息
            printAllHelp();
            break;
        case Cmd_Exit: // 只有课程退出才会返回false，其他情况均返回true
            return false;
            break;
        case Cmd_PrintCourse: // 打印课程信息
            _manager.printAllCourse();
            break;
        case Cmd_PrintCourseNum: // 打印课程数量
            cout << _manager.getCourseNum() << endl;
            break;
        case Cmd_PrintLongName: // 打印最长课程名称
            _manager.printLongNameCourse(); 
            break;
        case Cmd_RemoveLastCourse: // 删除最优一个课程
            _manager.removeLast();
            break;
        default: break;
    }
    return true;
}
