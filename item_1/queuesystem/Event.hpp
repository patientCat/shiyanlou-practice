//c++ program
//design by >^.^<
//@date    2018-10-11 16:06:21
#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include "Random.hpp" 

#define RANDOM_PARAMETER 100

struct Event{
    int occur_time;

    // 使用 -1 表示到达事件, >=0 表示离开事件， 同时数值表示所离开的服务窗口
    int event_type;

    Event *next;
    int id;
    // 默认为到达事件，发生时间随机
    // 在泊松分布中，lambda代表强度，即事件的每分钟发生次数
    Event(int occur_time = Random::getRandom(POISSON, 0.5), int etype = -1, int idd = 0)
    : occur_time(occur_time)
    , event_type(etype)
    , next(nullptr)
    , id(idd)
    {}
};

#endif /* Event.hpp */
