//c++ program
//design by >^.^<
//@date    2018-11-05 17:31:43

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *start_routine1(void *p)
{
    fprintf(stdout, "thread1: hello world\n");
    sleep(1);
    // 子线程1退出
    return NULL;
}


void *start_routine2(void *p2)
{
    fprintf(stdout, "thread2: im running...\n");
    pthread_t main_thread = (pthread_t)p2;
    pthread_t self = pthread_self();

    pthread_detach(pthread_self());
    if(pthread_equal(main_thread, self))
        fprintf(stdout, "main_threadID is equal to caller_threadID\n");
    else
        fprintf(stdout, "main_threadID is not equal to caller_threadID\n");
    pthread_join(main_thread, NULL);
    fprintf(stdout, "thread2: main thread exit!\n");

    fprintf(stdout, "thread2: exit!\n");
    fprintf(stdout, "thread2: process exit!\n");
    /*子线程2在此终止，进程退出*/
    pthread_exit(NULL);
}

int main()
{
    pthread_t pid1;
    pthread_t pid2;
    pthread_create(&pid1, NULL, start_routine1, NULL);
    pthread_join(pid1, NULL);
    pthread_create(&pid2, NULL, start_routine2, (void*)pthread_self());
    fprintf(stdout, "main_thread: is sleeping ...\n");
    sleep(3);
    fprintf(stdout, "main_thread: exit \n");
    pthread_exit(NULL);
    fprintf(stdout, "main_thread: you cannot see me \n");
    
}
