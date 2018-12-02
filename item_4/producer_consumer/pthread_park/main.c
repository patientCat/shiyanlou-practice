//c++ program
//design by >^.^<
//@date    2018-11-05 03:56:50

#include "park.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
#define ONE_SECOND 1000000
#define PERIOD 2000000
#define THREADS_NUM 4


void *car_in_handler(void *arg)
{
    CarPark_t *temp = (CarPark_t *)arg;
    unsigned int seed = rand();
    
    int factor = 1;
    pthread_barrier_wait(&temp->bar);
    while(1)
    {
        // 将线程随机挂起一段时间，模拟车辆到来的随机性
        usleep(rand_r(&seed) % (ONE_SECOND * factor));

        pthread_mutex_lock(&temp->lock); 
        // 处理业务
        // 循环等待，知道有停车位
        while(temp->occupied == temp->capacity)
        {
            pthread_cond_wait(&temp->hasspace, &temp->lock); // 这里使用while在linux编程手册中有详细记载
            factor = 4;
        }

        if(temp->occupied == 0)
            factor = 1;

        // 得到停车位后的处理
        ++temp->occupied;
        ++temp->cars_in;

        // 通知有人取车
        pthread_cond_signal(&temp->hascar);
        pthread_mutex_unlock(&temp->lock);
    }
    pthread_exit(NULL);
}

void *car_out_handler(void *arg)
{
    CarPark_t *park = (CarPark_t *)arg;
    unsigned int seed = rand();
    pthread_barrier_wait(&park->bar);

    int factor = 1;
    while(1)
    {
        usleep(rand_r(&seed) % (ONE_SECOND * factor));

        // 准备取车
        pthread_mutex_lock(&park->lock);

        // 先看看有没有车
        while(park->occupied <= 0)
        {
            pthread_cond_wait(&park->hascar, &park->lock);
            factor = 4;
        }

        if(park->occupied == park->capacity)
            factor = 1;

        // 取车操作
        ++park->cars_out;
        --park->occupied;
        pthread_cond_signal(&park->hasspace);

        pthread_mutex_unlock(&park->lock);
    }
    pthread_exit(NULL);
}

void *monitor(void *arg)
{
    // 观察停车场情况
    CarPark_t *park = (CarPark_t *)arg;
    while(1)
    {
        usleep(PERIOD);
        pthread_mutex_lock(&park->lock);

        fprintf(stdout, "停车场当前车数：%3d\n", park->occupied);
        fprintf(stdout, "进入停车场车辆：%3d, 驶出停车场车辆:%3d\n", park->cars_in, park->cars_out);

        pthread_mutex_unlock(&park->lock);
    }
}

static void initialize(CarPark_t *park, int capacity)
{
    park->capacity = capacity;
    pthread_mutex_init(&park->lock, NULL);
    pthread_cond_init(&park->hascar, NULL);
    pthread_cond_init(&park->hasspace, NULL);
    pthread_barrier_init(&park->bar, NULL, THREADS_NUM);
    park->occupied = 0;
    park->cars_in = 0;
    park->cars_out = 0;
    
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stdout, "Usage: %s carparksize\n", argv[0]);
        exit(-1);
    }

    srand(time(NULL));
    CarPark_t ourpark;

    initialize(&ourpark, atoi(argv[1])); // 初始化停车场数据结构

    pthread_t car_in, car_out;
    pthread_t car_in2, car_out2;
    pthread_t m;
    
    pthread_create(&car_in, NULL, car_in_handler, (void *)&ourpark);
    pthread_create(&car_out, NULL, car_out_handler, (void *)&ourpark);
    pthread_create(&car_in2, NULL, car_in_handler, (void *)&ourpark);
    pthread_create(&car_out2, NULL, car_out_handler, (void *)&ourpark);
    pthread_create(&m, NULL, monitor, (void *)&ourpark);

    pthread_join(car_in, NULL);
    pthread_join(car_out, NULL);
    pthread_join(car_in2, NULL);
    pthread_join(car_out2, NULL);
    pthread_join(m, NULL);
    return 0;
}
