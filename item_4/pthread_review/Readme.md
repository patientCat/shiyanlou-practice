# 复习多线程
## 为什么使用线程

> 同一进程内的线程共享绝大部分进程的资源。尤其是**`堆空间`**。少部分线程特有如**`线程栈空间`**。

多线程和多进程都可以实现并行任务。但<br/>
1. 进程间的信息难以共享。进程之间不共享内存。因此进程间通信需要采用IPC的方式进行信息交换。
2. fork的代价比较高，开销比较大。
3. 线程之间共享全局内存和堆内存。可以方便和快捷地共享信息。多线程共享一份资源时，需要涉及同步技术
即要使用锁。
4. 创建线程比创建进程通常要快10倍甚至更多。


## errno与返回值
所有的线程函数并不设置errno，因为多线程可能竞争资源的关系。所以其返回值为0代表函数成功，返回正值代表失败，其意义就是errno值。

## POSIX标准接口
### 线程的创建
```cpp
#include <pthread.h>
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
```
函数说明：<br/>
> POSIX thread 简称pthread; pthread_t是unsigned long类型的变量，用来表示线程的ID<br/>

函数参数：<br/>
> * thread（输出参数），由pthread_create在线程创建成功后返回的线程标识（或者线程句柄，其实就是线程的pid），该句柄在 后续操作线程的API中用于标志该新建的线程。如果新建线程想要获得自己的pid，建议使用`pthread_self()`，而不是和主线程 通信。<br/>
> * attr 设置线程属性，一般设置为NULL<br/>
> * start_routine 线程的入口函数<br/>
> * arg 用来给线程传递参数。<br/>

返回值：<br/>
> 成功返回0，失败返回相关错误码，注意这里不要使用perror，因为errno为全局变量，可能会发生竞争关系。最好的方式是自己 写一个类似perror的函数。

注意：<br/>
> * 主线程，这是一个进程的初始线程，其入口函数为main函数。<br/>
> * 新线程的运行时机不能被预测。即一个线程被创建之后有可能不会被马上执行，甚至，在创建它的线程结束后还没被执行；也 有可能新线程在当前线程从pthread_create返回前就已经在运行，甚至，可能在返回前已经结束。<br/>

在线程内，获取当前线程id
```cpp
pthread_t pthread_self(void);
```

比较俩个线程id是否相等，相同返回非0值，不同返回0。
```cpp
int pthread_equal(pthread_t t1, pthread_t t2);
```

### 线程的终止
线程终止分为俩种：被动终止和主动终止。<br/>
被动终止有俩种方式：<br/>
> 1. 线程的所在进程终止，任意线程执行exit函数，都会导致进程终止。从而依附于该进程的所有线程终止。
> 2. 其他线程调用pthread_cancel请求取消该线程（本质是发送信令）

主动终止也有俩种方式：<br/>
> 1. 线程的入口函数中执行return语句。或者main函数执行return语句会导致进程终止，从而结束所有线程（这个该算作被动终止）
> 2. 线程调用pthread_exit函数，main函数调用pthread_exit函数，主线程终止，但如果该进程内还有其它线程存在，进程会继续存在，进程内其它线程继续运行。

注意：再次强调主线程调用`return`和`pthread_exit()`区别<br/>

```cpp
#include <pthread.h>
void pthread_exit(void *retval);
```
> * pthread_exit会立即终止调用线程，不同于return之处在于可以在调用线程的任意函数内调用pthread_exit()结束线程
> * 其返回值通过`pthread_join`来获取
> * 参数retval指定了线程的返回值。但retval所指向的内容不应分配在线程栈中

### 可移植性
pthread_t在linux是以unsigned long实现，但无法保证其他操作系统实现。因为为了保证可移植性。请使用pthread_equal而不是简单使用关系运算符进行比较。

### 线程的连接
一个线程的终止对于另外一个线程而言是一种异步的事件，有时我们想等待某个ID的线程终止了再去执行某些操作，pthread_join函数为我们提供了这种功能，该功能称为线程的连接：<br/>
```cpp
include <pthread.h>
int pthread_join(pthread_t thread, void **retval);
```
参数说明
> * thread（输入参数），指定我们希望等待的线程
> * retval（输出参数），我们等待的线程终止时的返回值，就是pthread_exit中的参数

使用说明<br/>
当线程X`连接`线程Y时，如果线程Y仍在运行，则线程X会阻塞直到线程Y终止；如果线程Y在被`连接`之前已经终止了，那么线程X的`连接`调用会立即返回。因此，如果主线程调用`连接`，主线程就会阻塞并等待子线程终止<br/>

`连接`线程其实还有另外一层意义，一个线程终止后，如果没有人对它进行`连接`，那么该终止线程占用的资源，系统将无法回收，而该终止线程也会成为僵尸线程。因此，当我们去`连接`某个线程时，其实也是在告诉系统该终止线程的资源可以回收了。<br/>

**`注意`**:对于一个已经被`连接`过的线程再次执行`连接`操作， 将会导致无法预知的行为！

### 线程的分离
有时我们并不在乎某个线程是不是已经终止了，我们只是希望如果某个线程终止了，系统能自动回收掉该终止线程所占用的资源。相当于调用pthread_detach之后，就不用管那个线程了。系统会自动回收它。
```cpp
#include <pthread.h>
int pthread_detach(pthread_t thread);
```
**注意**: 一个线程如果已经分离了，就不能再连接了。


## 线程的属性
在线程的创建接口pthread_create的参数列表中有一个类型为pthread_attr_t的参数attr，该参数用于指定新建线程的相关属性。一个线程的属性主要包括：`线程栈的地址大小，线程的调度策略与优先级，还有线程是否处于分离状态等。`<br/>

线程属性的相关接口:<br/>
* 属性的初始化与销毁<br/>
```cpp
#include <pthread.h>
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);
```

`pthread_attr_t`函数必须在`pthread_create`函数之前调用<br/>
* 获取指定先的属性<br/>
```cpp
#define _GNU_SOURCE
#include <pthread.h>
int pthread_getattr_np(pthread_t thread, pthread_attr_t *attr);
```
记得事先使用宏扩展

* 设置和获取线程的分离状态

```cpp
#include <pthread.h>
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate);
```
* detachstate参数的取值：<br/>
    * PTHREAD_CREATE_JOINABLE 新建线程处于可连接状态，该值为不指定属性创建线程时的默认值
    * PTHREAD_CREATE_DETACHED 新建线程处于分离状态

* 设置和获取线程栈地址及大小：<br/>

```cpp
#include <pthread.h>
int pthread_attr_setstack(pthread_attr_t *attr,void *stackaddr, size_t stacksize);
int pthread_attr_getstack(pthread_attr_t *attr, void **stackaddr, size_t *stacksize);
```

# 线程安全问题
实现线程安全有多种方式<br/>
1. 在函数前后使用互斥量 优点在于简单，缺点在于将并行变成了串行
2. 将共享变量和互斥量关联起来，只有在使用到共享变量的时候才去使用互斥量，这就要求程序员来判断哪些部分是共享变量。

识别线程安全函数，一般在函数后加了`_r`就是线程安全函数。比如`rand_r()`
## 线程的一次性初始化
假设有以下需求，在我们的多线程程序中，我们几乎同时创建了N个线程（我们无法知道哪个线程会被先执行），我们期望这N个线程中谁先运行谁就要去负责去调用一个全局的初始化函数做相关的初始化工作，而该初始化函数仅能被调用一次。<br/>

接口如下:<br/>
```cpp
#include <pthread.h>
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
```
函数说明：该函数保证无论有多少个线程调用该函数，也仅仅会执行一次由init指向的函数实现<br/>
参数说明：<br/>
> * once_control 输入参数 必须这样静态初始化`pthread_once_t once = PTHREAD_ONCE_INIT; `对pthread_once的首次调用会修改once_control指向的内容，所以其不能设置为局部变量, 必须为全局变量，静态变量或者堆`
> * init 输入参数 即初始化函数的入口地址
> * 返回值 成功返回0 失败返回代表errno的正值（同其他posix标准下的函数一样）



## 线程特有数据
背景：实现线程安全的最有效的方式是使其可重入，即不使用全局变量和静态变量。然而对于已经存在不可重入函数库。我们没法修改其接口，因此发明此技术<br/>


所谓线程特有的数据，说白了，就死一块看起来是全局的数据概念，但是其实每个线程都有其特有的数据内容，因此每个线程都需要各自不受干扰的内存用来存放数据。<br/>
```cpp
#include <pthread.h>
int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));
int pthread_setspecific(pthread_key_t key, const void *value);
void *pthread_getspecific(pthread_key_t key);
```
函数说明：<br/>
1. pthread_key_create 创建全局唯一的key，用于表示一个数据概念
    * 因为进程中所有线程都需要使用这个key，所以需要设置为全局变量
2. pthread_setspecific 用于线程给某个数据概念分配内存
    * 将valuet提交后，就会和该线程以及该key绑定。成为仅仅只能被该线程访问的堆空间，即线程特有数据，且在该线程内分配一次后，后续就可以通过`pthread_getspecific`调用获得这块内存，相当于一块线程内的"全局"变量
3. pthread_getspecific 用于线程针对某个数据概念获取其对应的内存（每个线程获取的内存是不一样的），如果函数返回NULL值说明该线程还未对这个数据概念分配内存

接口使用思路如下：<br/>
1. 先用`pthread_key_create`创建一个key，用以区分不同函数使用的线程特有数据。只创建一次，因此需要借助`pthread_once`，key在创建时并未分配任何线程特有数据，同时要记得还有析构函数，当线程终止时，会将属于该线程的数据块指针传递给析构函数
2. 每个线程在使用该数据概念时，先通过`pthread_getspecified`查询该线程是否为该数据概念分配了内存
3. 如果线程未对该数据概念分配内存，使用`pthread_setspecified`为该数据概念分配特有内存
4. 如果线程已对该数据概念分配内存，直接操作该内存

注意:<br/>
* 由于一个数据概念对应一个key，即对于一个数据概念而言不管多少个线程`pthread_key_create`仅需要被调用一次，因此`pthread_create`经常在`pthread_once`函数里被调用
* `pthread_key_create`函数中有一个参数`destructor`，提供了一种释放线程特有数据内存的机制，当某个线程终止时，如果该线程针对该key分配了内存，那么`destructor`函数就会被调用，传递给`destructor`函数的参数就是该线程针对该key分配的内存指针

### 代码
使用线程特有数据，库函数执行步骤如下
```cpp
pthread_once_t once = PTHREAD_ONCE_INIT;     
pthread_key_t key                            
void destructor(void *arg)                   
{                                            
    free(arg);                               
}                                            
void init_routine()                          
{                                            
    pthread_key_create(&key, destructor);    
}                                            
void func()                                  
{                                            
    char *buf;                               
    pthread_once(&once, init_routine);       
                                             
    // 为每个调用者线程创建线程特有数据块，每个线程只分配一次
    buf = pthread_getspecific(key);// 得到关于特定的key，特定的线程关联的指针 
    if(NULL == buf)                          
    {                                        
        buf = (char *)malloc(size);// 这里也可以利用指针的空间特性在64编译器上存储一个long值 
        /*
            long save = 10;
            pthread_setspecific(key, (void *)save); // 类似 pthread_create传参时使用的用法
        */
        pthread_setspecific(key, buf);// 保存该buf指针，并将其和特定的key和特定的线程关联起来 
    }                                        
}                                            
```
## 示例程序
目的：POSIX系列标准的返回值为正值是errno值，我们希望定义自己的perror函数，根据POSIX函数得到的返回值在sys_errlist数组中查找错误描述，并返回。因此我们定义了一个全局的静态内存块buf。用它来接收返回字符串。<br/>
错误示例<br/>
在错误示例中，我们没有考虑线程安全的问题，因为全局内存对于多线程来说属于共享资源。所有存在竞争关系，而我们又不希望使用互斥量来同步。请看正确示例<br/>
```bash
gcc main.c myerror_notsafe.c -lpthread
```
正确示例
```bash
gcc main.c myerror.c -lpthread
```
