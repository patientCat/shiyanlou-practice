//c++ program
//design by >^.^<
//@date    2018-10-11 16:54:05

#include "QueueSystem.hpp" 

#include <cstdio>

#define TIMESLICE 10


QueueSystem::QueueSystem(int total_service_time, int window_num)
: _total_service_time(total_service_time)
, _window_num(window_num)
, _total_customer_stay_time(0)
, _total_customer_num(0)
{
    // 开辟_window_num个窗口
    this->_windows = new ServiceWindow[_window_num];
}  

QueueSystem::~QueueSystem()
{
    // 谁申请，谁删除
    delete [] _windows;
}

void 
QueueSystem::simulate(int simulate_num)
{
    double sum = 0;
    for(int i = 0; i != simulate_num; ++i)
    {
        sum += run();
    }
    // 每run一次代表的是一天, run返回的是顾客平均逗留时间。
    _avg_stay_time = static_cast<double>(sum) / simulate_num;
    // 这里略有疑问, 计算平均每分钟平均顾客数
    _avg_customers = static_cast<double>(_total_customer_num) / (_total_service_time * simulate_num);
}

// 系统初始化，初始化时间链表
void 
QueueSystem::init()
{
    // 第一个事件肯定是到达事件，到达时间为服从泊松分布, 使用默认事件，触发后续事件
    Event *event = new Event;
    _current_event = event;
}

// 系统开始运行，不断消耗事件表，当消耗完成时结束运行
double 
QueueSystem::run()
{
    init();
    while(_current_event)
    {
        // 判断当前事件类型, 并处理
        if(_current_event->event_type == -1)  
        {   // 到达事件
            customerArrived(); 
        }
        else{
            customerDeparture();
        }
        delete _current_event;
        // 获得新的事件
        _current_event = _event_queue.dequeue();
    };
    end();
    // 返回顾客的平均逗留时间
    return static_cast<double>(_total_customer_stay_time) / _total_customer_num;
}

// 系统运行结束，将所有服务窗口置为空闲，并清空用户的等待队列和事件列表

void QueueSystem::end()
{
    // 设置所有窗口空闲
    for(int i = 0; i != _window_num; ++i)
        _windows[i].setIdle();

    // 顾客队列清空
    _customer_queue.clear();

    // 事件列表清空
    _event_queue.clear();
}

// 利用宏观的概率分布，由第一个事件来触发后续的事件。
void 
QueueSystem::customerArrived()
{
    if(_current_event->event_type == -1) 
    {
        ++_total_customer_num;
        // 生成下一个顾客的到达事件, 下一个顾客假设 按照泊松分布的时间到达 
        int intertime = Random::getRandom(POISSON, 0.5);
        // 下一事件的发生时间为当前时间 + 下一个顾客到达时间
        int time = _current_event->occur_time + intertime;  
        // 插入新的事件
        Event temp_event(time);
    // 如果下一个顾客的到达时间小于服务的总时间，就把这个事件插入到事件列表中
    // 同时将这个顾客加入到customer_list 进行排队
    if(time < _total_service_time)
        _event_queue.eventEnqueue(temp_event);
    // 否则不列入事件表，且不加入 customer_queue , 如果一个事件没有进入事件队列，那么其也不会产生顾客

    // 处理当前事件中到达的顾客, 基于当前事件来产生顾客
    Customer customer = Customer(_current_event->occur_time);
    // 当前顾客肯定要被压入顾客队列中
    _customer_queue.enqueue(customer);
    }
    else{
        // 需要知道旧的顾客的duration 时间, 因此需要建立一个缓存机制
        Customer oldcustomer = Customer(_current_event->occur_time, _oldcustomer_buff[_current_event->id]); 
        _customer_queue.enqueue(oldcustomer);
    }

    Customer *pcustomer = nullptr;
    // 如果当前窗口有空闲窗口，那么直接将这个顾客送入服务窗口
    int idleIndex = getIdleServiceWindow(); 
    if(idleIndex >= 0)
    {
        pcustomer = _customer_queue.dequeue();
        _windows[idleIndex].serveCustomer(*pcustomer);
        _windows[idleIndex].setBusy();

        // 当顾客到窗口开始服务时，就需要插入这个顾客的一个离开时间到event_list
        // 离开事件的发生时间 = 当前事件的发生时间 + 服务时间

        Event temp_event(_current_event->occur_time + pcustomer->duration, idleIndex);
        _event_queue.eventEnqueue(temp_event);
    }
    delete pcustomer;
}  

// 获得空闲窗口的索引
int 
QueueSystem::getIdleServiceWindow()
{
    for(int i = 0; i != _window_num; ++i)
    {
        if(_windows[i].isIdle())
        {
            return i;
        }
    }
    return -1;
} 

// 处理用户离开事件
void QueueSystem::customerDeparture()
{
    // 如果离开事件的发生时间比服务时间大， 我们就不需要做任何处理
    if(_current_event->occur_time < _total_service_time)
    {
        // 顾客逗留时间 = 时间片常数时间
        _total_customer_stay_time += TIMESLICE < _windows[_current_event->event_type].getCustomerDuration() ? TIMESLICE :  _windows[_current_event->event_type].getCustomerDuration();
        // 如果当前服务的顾客 duration < 时间片时间，则此顾客完成任务
        // 直接被销毁，否则就应该将当前顾客插入到等待队列中
        // 同时，因为这是事件驱动，需要创建一个新的事件插入到事件列表中
        // 其发生时间是当前队列队尾顾客发生时间+时间片时间
        if(_windows[_current_event->event_type].getCustomerDuration() > TIMESLICE)
        {
            // 插入新的事件
            int temp_duration = _windows[_current_event->event_type].getCustomerDuration();
            int temp_time;
            if(0 == _customer_queue.length()) // 如果此时已经没有顾客等待
            {
                temp_time = _current_event->occur_time + TIMESLICE;
            }
            else{
                temp_time = _customer_queue.getLastCustomer()->arrive_time + TIMESLICE;
            }
            // -2代表旧的进程
            Event temp_event(temp_time, -2, _oldcustomer_buff.size());
            _event_queue.eventEnqueue(temp_event);

            /*
            // 将当前顾客重新插入到等待队列
            Customer customer(temp_time, temp_duration - TIMESLICE);
            _customer_queue.enqueue(customer);
            */
            // 这里不插入顾客，而是将这个旧的顾客放到缓存之中
            _oldcustomer_buff.push_back(temp_duration - TIMESLICE);
        }
        // 如果队列中有人等待，则立即去服务
        if(_customer_queue.length())
        {
            Customer *customer;
            customer = _customer_queue.dequeue();
            _windows[_current_event->event_type].serveCustomer(*customer);

            // 离开事件
            Event temp_event(_current_event->occur_time + customer->duration, _current_event->event_type);
            _event_queue.eventEnqueue(temp_event);
            delete customer;
        }
        else{
            // 如果队列没有人，且当前窗口的顾客离开了，则这个窗口是空闲的
            _windows[_current_event->event_type].setIdle();
        }
    }
}    
                
