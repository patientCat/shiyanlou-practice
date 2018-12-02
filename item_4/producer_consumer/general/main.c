//c++ program
//design by >^.^<
//@date    2018-11-07 10:14:29

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
    int tickets;
    int windows;
    pthread_mutex_t mutex;
    pthread_cond_t hastickets;
    pthread_cond_t notickets;
}Tickets_t;

// 模拟火车站售票

void initTickets(Tickets_t *ptickets, int w)
{
    ptickets->tickets = rand() % 10 + 10; 
    ptickets->windows = w;
    pthread_mutex_init(&ptickets->mutex, NULL);
    pthread_cond_init(&ptickets->hastickets, NULL);
    pthread_cond_init(&ptickets->notickets, NULL);
}

void *sell_routine(void *arg)
{
    Tickets_t * ptickets = (Tickets_t *)arg;
    while(1)
    {
    pthread_mutex_lock(&ptickets->mutex);
    while(ptickets->tickets > 0)
        pthread_cond_wait(&ptickets->notickets, &ptickets->mutex);

    fprintf(stdout, "加票中ing\n");
    sleep(3);
    fprintf(stdout, "加票中done\n");
    ptickets->tickets = rand() % 10 + 10;
    fprintf(stdout, "火车站现在有票%3d张\n", ptickets->tickets);
    pthread_cond_signal(&ptickets->hastickets);
    pthread_mutex_unlock(&ptickets->mutex);
    }
    pthread_exit(NULL);
}

void *buy_routine(void *arg)
{
    Tickets_t *ptickets = (Tickets_t *)arg;

    while(1)
    {
    pthread_mutex_lock(&ptickets->mutex);
    while(0 == ptickets->tickets)
        pthread_cond_wait(&ptickets->hastickets, &ptickets->mutex);

    --ptickets->tickets;
    fprintf(stdout, "站点:%ld卖出去一张票，还余票%d。\n", (long)pthread_self() , ptickets->tickets);
    if(0 == ptickets->tickets)
        pthread_cond_signal(&ptickets->notickets);

    pthread_mutex_unlock(&ptickets->mutex);
    sleep(1);
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if(2 != argc)
    {
        fprintf(stdout, "please use %s windows_num\n", argv[0]);
        exit(-1);
    }
    int windows = atoi(argv[1]);

    pthread_t sell;
    pthread_t buy[10];
    srand(time(NULL));
    Tickets_t tickets;
    
    initTickets(&tickets, windows);


    fprintf(stdout, "火车票开始售票，当前票数为%3d张, 售票窗口为%3d个\n", tickets.tickets, tickets.windows);
    pthread_create(&sell, NULL, sell_routine, (void *)&tickets);
    for(int i = 0; i < windows; ++i)
    {
        pthread_create(&buy[i], NULL, buy_routine, (void *)&tickets);
    }

    pthread_join(sell, NULL);
    for(int i = 0; i < windows; ++i)
    {
        pthread_join(buy[i], NULL);
    }
    
    return 0;
}

