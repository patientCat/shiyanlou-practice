//c++ program
//design by >^.^<
//@date    2018-10-14 22:14:44

#ifndef _cmdmanager_hpp_
#define _cmdmanager_hpp_

#include "CourseManager.hpp" 
#include "Cmd.hpp"

#include <map> 
#include <string> 

using std::map;
using std::string; 

class CmdManager 
{ 
public: 
    CmdManager () = default; 
    ~CmdManager () = default;

    // 初始化函数，初始化支持的命令及课程信息
    void init();

    // 打印帮助信息
    void printAllHelp() const;

    // 根据命令查询帮助信息
    void printCmdHelp(const CourseCmd cmd) const;
     
    // 处理命令操作
    bool handleCmd(const CourseCmd cmd);

    // 返回课程管理对象
    CourseManager& getCourseManager() { return _manager; }
private: 
    CourseManager _manager;
    map<CourseCmd, string> _cmdMap;
}; 
#endif /* CmdManager_hpp */
