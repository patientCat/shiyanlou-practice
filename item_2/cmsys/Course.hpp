//c++ program
//design by >^.^<
//@date    2018-10-13 20:39:22

#ifndef _COURSE_H_
#define _COURSE_H_

#include <string> 
#include <iostream> 

using std::string; 
using std::ostream; 
using std::istream; 

// 存储和处理课程信息
class Course 
{ 
friend ostream& operator<<(ostream &out, const Course &rvalue);
// 读取输入，创建新的课程
friend void read(istream &in, Course &course);
public: 
    Course (); 
    Course (const string &cName)
    : Course()
    {
        _name = cName;
    }
    ~Course () = default;
     
    // 打印课程信息
    virtual void printInfo(ostream &os) const ;

    string getName() const {    return _name;    }

    void setName(const string &newName) {    _name = newName;   }

    int getId() const { return _id; }

protected: 
    int _id;
    string _name;

    static int currentId;
}; 

class ProjectCourse 
: public Course
{ 
public: 
    void setTag(const string &newTag)   {   _tag = newTag;   };
     
    string getTag() const   {   return _tag;    }

    virtual void printInfo(ostream &os) const override;
protected: 
    string _tag;
}; 

class EvaluationCourse
: public Course
{ 
public: 
    void setTime(const double &newTime)     {   _time = newTime;    }

    double getTime() const  {   return _time;   }
     
    virtual void printInfo(ostream &os) const override;
protected:
    double _time;
}; 
#endif /* Course_h */
