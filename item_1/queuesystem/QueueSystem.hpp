//c++ program
//design by >^.^<
//@date    2018-10-11 16:29:36
#ifndef _QUEUESYSTEM_HPP_
#define _QUEUESYSTEM_HPP_

#include "Event.hpp" 
#include "Queue.hpp" 
#include "ServiceWindow.hpp"
#include <vector> 

using std::vector; 

class QueueSystem 
{ 
public: 
    QueueSystem (int total_service_time, int window_num);
    ~QueueSystem ();
     
    void simulate(int simulate_num);

    double getAvgStayTime() const {     return _avg_stay_time;  }
    double getAvgCustomers() const {    return _avg_customers;  }

private:
    double run();

    void init();

    void end();

    // 获得空闲窗口索引
    int getIdleServiceWindow();

    // 处理顾客到达事件
    void customerArrived();

    // 处理顾客离开事件
    void customerDeparture();

    // 总的营业时间
    int _total_service_time;

    // 服务窗口的总数
    int _window_num;

    // 顾客逗留总时间
    int _total_customer_stay_time;

    // 总顾客数目
    int _total_customer_num;

    double _avg_customers;
    double _avg_stay_time;

private:  // 核心成员
    ServiceWindow *_windows;
    Queue<Customer> _customer_queue;
    Queue<Event> _event_queue;
    Event*  _current_event;
    vector<int> _oldcustomer_buff;
}; 

#endif /* QueueSyste_hpp */
