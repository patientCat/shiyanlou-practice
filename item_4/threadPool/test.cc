//c++ program
//design by >^.^<
//@date    2018-10-15 16:25:31
#include <iostream>

using namespace std;

int main()
{
    int val1 = 1;
    int val2 = 2;
    auto lambda = [&]()mutable{
        ++val1; ++val2; };
    lambda();
    cout << val1 << "  " << val2 << endl;
    return 0;
}
