//c++ program
//design by >^.^<
//@date    2018-10-15 09:08:08
#include "CmdManager.hpp" 
#include <iostream>

using namespace std;

int main()
{
    CmdManager cmdManager;
    cmdManager.init();

    // 打印帮助信息
    cmdManager.printAllHelp();

    int cmd;
    while(cin >> cmd)
    {
        if(cin.good())
        {
            bool exitCode = cmdManager.handleCmd((CourseCmd)cmd);
        if(!exitCode)
            return 0;
        }

        cout << "-----------------------------------" << endl;
        cout << "New Command:";

        cin.clear();
        cin.ignore();
    }
    return 0;
}
