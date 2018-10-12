//c++ program
//design by >^.^<
//@date    2018-10-11 15:46:08
#ifndef _NODE_HPP_
#define _NODE_HPP_

#include "Random.hpp" 

#define RANDOM_PARAMETER 100
#define DEFAULT_RANDOM_DURATION 12

struct Node{
    // 默认到达时间为0，需要服务的时间是半天的随机
    // 指数分布表示直到某件事情的平均时间，这里假设的平均服务时间为10分钟。那么服务时间就服从指数分布
    Node(int a_time = 0, int d = Random::getRandom(EXPONENTIAL, 0.1))
    : arrive_time(a_time),
    duration(d),
    next(nullptr)
    {}

    int arrive_time; // 到达时间
    int duration; // 顾客任务时间，即在银行待多少时间
    Node* next;
};

using Customer = Node;
#endif /* Node_hpp */
