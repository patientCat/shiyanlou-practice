#ifndef _PARK_H_
#define _PARK_H_

#include <pthread.h>

typedef struct{
    int capacity; // 停车场的容量限制
    int occupied; // 现有停车数目
    int cars_in; // 记录停车场进入车辆的总和
    int cars_out; // 记录从停车场开出去的车辆总和
    pthread_mutex_t lock; // 互斥锁，保护停车场
    pthread_cond_t hasspace; // 条件变量，
    pthread_cond_t hascar;
    pthread_barrier_t bar;
}CarPark_t;
#endif // park.h
