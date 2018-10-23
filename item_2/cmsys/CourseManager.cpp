//c++ program
//design by >^.^<
//@date    2018-10-14 21:43:22
#include "CourseManager.hpp" 

#include <iostream> 
using std::cout; 
using std::cin; 
using std::endl; 
using std::cerr; 

CourseManager::CourseManager (const vector<Course> &courseVec)
{
    _courseList = courseVec;
}

void 
CourseManager::addCourse(const Course &course)
{
    _courseList.push_back(course);
}

void 
CourseManager::addCourse(const string &courseName)
{
    Course tmp(courseName);
    _courseList.push_back(tmp); 
}

void 
CourseManager::removeLast()
{
    if(_courseList.size())
    {
        _courseList.pop_back();
        cout << "剩余课程: " << _courseList.size() << endl;
    }
    else
        cerr << "列表已经为空了，不能再删除了" << endl;
}

void 
CourseManager::removeById(int id)
{
    _courseList.erase(std::find_if(_courseList.begin(), _courseList.end(), [=](Course &c)->bool{
                                                          if(c.getId() == id) return true;
                                                          return false; }));
}

void 
CourseManager::printAllCourse()
{
    for(auto &a : _courseList)
    {
        a.printInfo(cout);
        cout << endl;
    }
}


void 
CourseManager::printCourse(const string &name)
{
    auto ret = std::find_if(_courseList.begin(), _courseList.end(), [&](Course &c)->bool{
                                                            if(c.getName() == name) return true;
                                                            return false;   });  
    if(ret != _courseList.end())
    {
        ret->printInfo(cout); 
        cout << endl;
    }
    else
        cerr << name << " Not Found" << endl;
}

void 
CourseManager::printCourse(int id)
{
    auto ret = std::find_if(_courseList.begin(), _courseList.end(), [&](Course &c)->bool{
                                                            if(c.getId() == id) return true;
                                                            return false;   });  
    if(ret != _courseList.end())
    {
        ret->printInfo(cout); 
        cout << endl;
    }
    else
        cerr << id << " Not Found" << endl;
}

void 
CourseManager::printLongNameCourse()
{
    int m = 0;
    vector<Course>::iterator ret ;
    for(auto begin = _courseList.begin(); begin != _courseList.end(); ++begin)
    {
        if((int)begin->getName().size() > m)
        {
            m = begin->getName().size(); 
            ret = begin;
        } 
    }
    ret->printInfo(cout);  cout << endl;
}
