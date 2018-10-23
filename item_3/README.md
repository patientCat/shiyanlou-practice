# 简单群聊服务器
## 需求
客户端<br/>
1. 连接服务器
2. 支持用户输入聊天消息，发送消息给服务器
3. 接收并显示服务器的消息

服务器<br/>
1. 接收客户端消息
2. 然后进行广播

# 知识复习
服务器端：
1. socket()创建监听Socket
2. bind()绑定服务器端口
3. listen()监听客户端连接
4. accept()接受连接
5. recv/send接收以及发送数据
6. close()关闭socket

客户端：
1. socket()创建监听Socket
2. connect()连接服务器
3. recv/send接收以及发送数据
4. close()关闭socket

### TCP服务器端通信的常规步骤
1. 使用socket()创建TCP套接字，会得到一个套接字描述符
2. 将创建的套接字绑定到一个本地地址和端口上（bind）
3. 将套接字设为监听模式，准备接收客户端请求（listen）
4. 登台客户请求的到来：当请求到来后，接受连接请求，返回一个对应于此次连接的新的套接字描述符（accept）
5. 使用新得到的套接字和客户端进行通信
6. 返回等待另一个客户请求
7. 关闭套接字

```cpp
#define Perror_less(val, check_val, note)    {\
if(val < check_val) \
{   printf(note); exit(-1); }\
}
#define Perror_greater(val, check_val, note)    {\
if(val > check_val) \
{   printf(note); exit(-1); }\
}
#define Perror_equal(val, check_val, note)    {\
if(val == check_val) \
{   printf(note); exit(-1); }\
}
                                    
// Server.cpp代码
// 服务端地址 = ip地址 + 端口号
struct sockaddr_in serverAddr;
serverAddr.sin_family = PF_INET; //AF_INET对应于BSD标准, PF_INET对应于Posix标准
serverAddr.sin_port = htons(SERVER_PORT); // 设置端口号，主机字节序需要转网络序
serverAddr.sin_addr.s_addr = inet_addr(SERVER_HOST); // 设置ip地址

// 服务器端监听socket
int listenfd = socket(PF_INET, SOCK_STREAM, 0);
Perror_less(listenfd, 0, "listen fd");
printf("listen socket created \n");

// 将服务端地址与监听socket绑定
int ret = bind(listenfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
Perror_less(ret, 0, "bind error");

// 开始监听
int ret = listen(listenfd, 5);
Perror_less(ret, 0, "listen error");
```

### TCP客户端通信的常规步骤
1. 创建套接字（socket）
2. 使用connect()建立到达服务器的连接（connect）
3. 客户端进行通信，使用send和recv
4. 使用close()关闭客户连接

```cpp
// Client.cpp
// 客户要连接的!!!!服务端地址!!!! = ip地址 + 端口号 （看清楚了）
struct sockaddr_int serverAddr;
serverAddr.sin_family = PF_INET;
serverAddr.sin_port = htons(SERVER_PORT);
serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

int sockfd = socket(PF_INET, SOCK_STREAM, 0);
Perror_less(sockfd, 0, "sock error");
int ret = connect(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
Perror_less(ret, 0, "connect error");
```

### 阻塞与非阻塞socket
通常一个文件描述符工作有俩种方式，一种是阻塞，另外一种是非阻塞。<br/>
1. 阻塞方式是指：当试图对该文件描述符进行读写时，如果当时没有数据可读，或者暂时不可写，程序就进入等待状态。
直到有东西可读或者可写为止。<br/>
2. 非阻塞方式是指：如果没有数据可读，或者不可写，读写函数马上返回，而不会等待。<br/>

> 阻塞与非阻塞事实上与读写无关，主要是文件描述符自己的设置。<br/>
> 其主要区别在于是否`立即返回`。<br/>

```cpp
// 将文件描述符设置为非阻塞方式
fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
```
> 解释：先获得当前sockfd的状态，

### epoll多路复用
1. epoll之所以高效，是因为epoll将用户关心的文件描述符放到内核的一个事件表中，而不是像select/poll每次调用都需要
重复传入文件描述符或事件集。比如当一个事件发生时，epoll无需遍历整个被侦听的描述符集，只要遍历那些被内核IO事件
异步唤醒而加入就绪队列的描述符集合就行了。
2. epoll有俩种工作方式，LT水平触发和ET边沿触发。水平触发，即只要满足状态就会一直触发，边沿触发，只有状态变化
才会触发。且只触发一次。
    * 举例：比如来了1000字节，LT和ET都会被触发，LT是因为监听的描述符可读了，所以会触发，ET是因为监听的描述符由
    * 不可读变为可读了，状态发生变化才会触发。所以如果我们读取100字节后。LT依然会触发，因为描述符依然处于可读
    * 状态，而ET不会触发，因为`可读->可读`并没有发生状态变化。

#### epoll函数
创建epoll句柄
```cpp
int epfd = epoll_create(1);
```
epoll事件注册函数
```cpp
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```
1. 第一个参数epfd为epoll_create返回的句柄，其本质是一颗RBTree
2. 第二个参数为对时间的操作，其中fd-event 就是RBTree的key-value
    * Epoll_ctl_add 注册新的fd所关联的事件到epfd，即RBTree的插入
    * Epoll_ctl_del 删除一个fd所关联的时间
    * epoll_ctl_mod 修改一个fd所关联的事件

实例
```cpp
struct epoll_event ev;
ev.data.fd = fd; // 将fd和事件关联
ev.events = EPOLLIN; // 监听读事件
if(enbale_ET)
    ev.events |= EPOLLET;
epoll_ctl(epfd, Epoll_ctl_add, fd, &ev); // 注意这里的fd为key值，用来寻找所对应的事件，所以一定要和事件关联的fd一致
setNonBlocking(fd);
```

epoll等待事件的产生
```cpp
// 等待事件的产生
int epoll_wait(int epfd, struct epoll_events *events, int maxevents, int timeout);
```
1. epfd:由epoll_create 生成的epoll专用的文件描述符；
2. epoll_event:用于回传代处理事件的数组；
3. maxevents:每次能处理的事件数；
4. timeout:等待I/O事件发生的超时值（ms）；-1永不超时，直到有事件产生才触发，0立即返回。
5. 返回值：返回就绪或者发生的事件数。-1有错误。


# 项目升级
实现一个真正的通信聊天室程序
1. 连接服务器端时，输入昵称替代数字ID
2. 私聊功能，通过命令行指定发给哪个客户端
3. 增加多线程提高并发量的支持
4. 增加超时重传和确认收包等功能增强稳定性

先学习下知识再回来做这部分任务
