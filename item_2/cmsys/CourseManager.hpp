//c++ program
//design by >^.^<
//@date    2018-10-14 21:32:45
#ifndef _COURSEMANAGER_H_
#define _COURSEMANAGER_H_

#include "Course.hpp" 
#include <vector> 

using std::vector; 
using std::string; 

class Course;

class CourseManager 
{ 
public: 
    CourseManager () = default;
    // 根据vector创建CourseManager
    CourseManager (const vector<Course> &courseVec);
    ~CourseManager () = default;

    // 获取课程列表长度
    int getCourseNum() const {  return _courseList.size();  }

    // 添加新的课程
    void addCourse(const Course &course);
    void addCourse(const string &courseName);
     
    // 删除课程:删除最后一个课程
    void removeLast();

    // 删除课程:删指定ID的课程
    void removeById(int id);

    // 打印课程列表信息
    void printAllCourse();

    // 根据课程名称打印指定课程信息
    void printCourse(const string &name);

    // 根据课程ID打印指定课程信息
    void printCourse(int id);

    // 打印名称最长的课程
    void printLongNameCourse();
private: 
    // 存储课程列表
    vector<Course>	_courseList; 
}; 
#endif /* CourseManager_h */
