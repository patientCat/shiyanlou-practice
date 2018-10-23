//c++ program
//design by >^.^<
//@date    2018-10-13 21:09:27

#include "Course.hpp" 
#include <iostream> 


using std::cout; 

// 课程id自动生成且增长
int Course::currentId = 0; 

Course::Course()
{
    _id = ++currentId;
    _name = "";
} 

ostream& operator<<(ostream &out, const Course &course)
{
    course.printInfo(out);
    return out;
}

void 
Course::printInfo(ostream &os) const
{
    os << "Course: " << _id << " : " << _name;
}

void 
ProjectCourse::printInfo(ostream &os) const
{
    os << "Course: " << _id << " : " << _name << " tag: " << _tag;
}

void
EvaluationCourse::printInfo(ostream &os) const
{
    os << "Course: " << _id << " : " << _name << " time: " << _time;
}
