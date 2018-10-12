//c++ program
//design by >^.^<
//@date    2018-10-11 15:30:39
#include "QueueSystem.hpp" 

#include <iostream>
#include <cstdlib> 
// CPU 资源争夺

int main()
{
    using namespace std;
    std::srand(time(0));
    // CPU总的服务时间
    int total_service_time = 240;
    // CPU核数 
    int window_num = 4;
    // 模拟次数
    int simulate_num = 1000;
    QueueSystem system(total_service_time, window_num);
    system.simulate(simulate_num);

    std::cout << "The average time of customer stay in bank: " << system.getAvgStayTime() << std::endl;
    std::cout << "The number of customer arrive bank per minute: " << system.getAvgCustomers() << std::endl;     
    return 0;
}
