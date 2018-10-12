//c++ program
//design by >^.^<
//@date    2018-10-11 16:18:04
#ifndef _QUEUE_HPP_
#define _QUEUE_HPP_

#include "Event.hpp" 
#include <cassert> 

template <typename T>
class Queue
{
public:
    Queue()
    : _front(new T())
    , _rear(_front)
    , _length(0)
    {
        _front->next = nullptr; 
    }

    ~Queue()
    {
        if(_length)
            clear();
        delete _front;
    }

    // 清空队列
    void clear();
    T * enqueue(T &node);
    T * dequeue();
    // 事件入队
    T * eventEnqueue(Event &event); 
    int length() const  {   return _length; }
    T * getLastCustomer()   {   return _rear;   }

private:
    T           *_front;
    T           *_rear;
    int         _length;
};

template <typename T>
T *
Queue<T>::enqueue(T &node)
{
    T *new_node = new T();
    *new_node = node;

    _rear->next = new_node;
    _rear = new_node;  
    ++_length;
    return new_node;
} 

template <typename T>
T *
Queue<T>::dequeue() 
{
    if(!_length)
        return nullptr;
    assert(_length > 0);
    --_length;
    T *tmp_node = _front->next; 
    _front->next = tmp_node->next;  
    // 如果删除最后一个节点，尾指针要回指头指针
    if(!_length)
        _rear = _front;
    return tmp_node;
}

// 事件是按照发生的先后顺序入队
template <typename T>
T *
Queue<T>::eventEnqueue(Event &event)
{
    Event *tmp = new Event();
    *tmp = event;
    ++_length;
    if(!_length)
    {
        _rear->next = tmp;
        _rear = tmp;
        return tmp;
    }
    Event * scan = _front;
    while(scan->next && scan->next->occur_time < event.occur_time)
        scan = scan->next;
    tmp->next = scan->next;
    scan->next = tmp;     

    return tmp;
}

// 清空队列中所有元素
template <typename T>
void
Queue<T>::clear()
{
    if(!_length)
        return ;
    T *scan = _front->next; 
    T *pre = _front;
    while(scan)
    {
        pre = scan;
        scan = scan->next;
        delete pre;
    }
    _front->next = nullptr; 
    _rear = _front;
    _length = 0;
} 
#endif /* Queue_hpp */
