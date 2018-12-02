# 线程同步
## 互斥量
这个比较好理解
## 条件变量
条件变量允许一个线程就某个共享变量（或者资源）的状态变化通知其他线程，并让其他线程等待这一通知。<br/>
条件变量总是结合互斥量一起使用。<br/>

### 初始化条件变量
俩种方式，一种静态，一种动态。<br/>
* 静态
    * `pthread_cond_t cond = PTHREAD_COND_INITIALIZER;`
* 动态
    * `pthread_cond_init(&cond, NULL);`

### 通知条件变量
```cpp
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
```
signal使用环境为所有等待线程执行相同任务，即从wait中被唤醒后，执行的是相同任务，因此不论唤醒哪个线程都可以。<br/>
broadcast使用环境相反，处于等待状态的线程任务不同。<br/>

**注意** 条件变量并不保存状态信息。发送信号时，如果没有任何线程在等待此条件变量，这个信号会不了了之。线程如在此后等待条件变量，只有当再次收到此变量的下一信号时，方可解除阻塞状态。<br/>

### 等待条件变量
```cpp
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
```

详细的解释：<br/>
> pthread_cond_wait(&cond, &mutex) 所做的操作如下<br/>
> * 解锁互斥量mutex
> * 阻塞调用线程，直到收到关于此cond的signal
> * 重新锁定mutex

### 典型的生产者消费者模型
#### 生产者
```cpp
pthread_mutex_lock(&mtx);

// 检查共享变量不是我们想要的状态
while(product == capacity) // 如果仓库满了，就在此等待
{
    pthread_cond_wait(&hasspace, &mtx);
}
// 生产产品
++product;
pthread_cond_signal(&hasproduct, &mtx); // 通知消费者，有产品了

pthread_mutex_unlock(&mtx);
```

#### 消费者
```cpp
pthread_mutex_lock(&mtx);

// 检查共享变量不是我们想要的状态
while(product == 0) // 如果没有产品，就在此等待
{
    pthread_cond_wait(&hasproduct, &mtx);
}

--product;
pthread_cond_signal(&hasspace, &mtx); // 通知生产者，仓库有空间了，可以生产产品了

pthread_mutex_unlock(&mtx);
```
1. 线程在准备检查共享变量之前，必须锁定互斥量
2. 检查共享变量
3. 如果共享变量未处于预期状态，线程应在等待条件变量并进入休眠前解锁互斥量（以便其他线程访问该共享变量）
4. 当线程因为条件变量的通知而被再度唤醒时，必须对互斥量再次加锁，因为之后典型情况下回访问共享变量。这里称为蹭锁。

其中3，4步会由pthread_cond_wait自动执行，这里细节是第3步互斥量的释放与陷入休眠属于一个原子操作。即只有在当前调用线程陷入休眠开始等待条件变量后，其他线程才有可能获取到该互斥量。<br/>
先通知条件变量和先解锁互斥量没有先后顺序，怎样都可以。<br/>

#### 为什么使用while判断
1. 某些实现下，存在虚假唤醒，即条件变量被异常唤醒
2. 其他线程率先醒来。和当前线程抢锁，抢到了锁，并修改了共享变量的状态。这样在当前线程再加锁执行的时候，共享变量可能已经不再满足我们期望的状态了
