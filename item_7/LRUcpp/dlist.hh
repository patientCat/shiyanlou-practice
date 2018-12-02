#ifndef _DLIST_H_
#define _DLIST_H_

#include "lruentry.hh" 

#include <cstdio>

using DLNode = CacheEntry;
class DList
{
public:
    DList()
    {
        init();
    }
    ~DList()
    {
        delete _head;
    }
    DLNode *insertAsHead(DLNode * pnode)
    {
        DLNode * ret = nullptr;
        if(_tail == _head)
        {
            ret = pnode->insertDListAfter(_head);
            _tail = ret;
        }
        else{
            ret = pnode->insertDListAfter(_head);
        }
        return ret;
    }
    
    DLNode *removeFromLsit(DLNode * pnode)
    {
        if(_tail == _head)
            return nullptr;
        pnode->dListPrev->dListNext = pnode->dListNext;
        if(pnode->dListNext)
            pnode->dListNext->dListPrev = pnode->dListPrev;
        return pnode;
    }

    DLNode *removeTail()
    {
        if(_tail == _head)
            return nullptr;
        DLNode * removed = _tail;
        _tail = _tail->dListPrev;
        _tail->dListNext = nullptr;
        return removed;
    }
    void printList() const
    {
        DLNode * scan = _head;
        while(nullptr != (scan = scan->dListNext))
        {
            printf("%3c", scan->data);
        }
    }
    bool empty() const  {   return _tail == _head;  }
private:
    void init()
    {
        _head = new DLNode();
        _tail = _head;
    }

private:
    DLNode * _head;
    DLNode * _tail;
};
#endif
