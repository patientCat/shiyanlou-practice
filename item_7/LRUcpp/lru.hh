#ifndef _LRU_HH_
#define _LRU_HH_
#include "lruentry.hh" 
#include "dlist.hh"
#include "hashMap.hh"

// 因为大多数情况下基本用到的
class Cache{
public:
    Cache(int capacity)
    : _capacity(capacity)
    , _size(0)
    , _hashMap(capacity)
    {
        init();
    }

    ~Cache()
    {

    }

    /* 
     * 功能：将数据放入缓存中 
     * dList需要插入表项，需要注意size > capacity
     * hashMap需要插入表项
     */
    int LRUCacheSet(KeyType key, DataType data )
    {
        CacheEntry * entry = new CacheEntry(key, data);
        CacheEntry * toRemoveEntry = nullptr;
        // 如果size超出缓存表capacity
        if(++_size > _capacity)
        {
            toRemoveEntry = _dlist.removeTail(); // 删除最远没有使用的entry
            _hashMap.remove(toRemoveEntry);
            delete toRemoveEntry;
        }
        _dlist.insertAsHead(entry);
        _hashMap.insert(entry);
        return 0;
    }

    /*
     * 功能：从缓存中获取数据
     */
    DataType LRUCacheGet(KeyType key)
    {
        CacheEntry * entry = _hashMap.find(key);
        _dlist.removeFromLsit(entry);
        _dlist.insertAsHead(entry);
        if(!entry)
        {
            return static_cast<DataType>(0);
        }
        else{
            return entry->data;
        }
    }

    // 测试使用
    void print()
    {
        _dlist.printList();
    }

private:
    void init()
    {
    }
private:
    int _capacity;
    int _size;
    DList _dlist;
    HashMap _hashMap;
};
#endif
