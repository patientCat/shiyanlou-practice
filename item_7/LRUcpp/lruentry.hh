#ifndef _LRUENTRY_H_
#define _LRUENTRY_H_

using DataType = char;
using KeyType = int;
struct CacheEntry{
    CacheEntry(KeyType kk, DataType dd, CacheEntry *dPrev = nullptr, CacheEntry *dNext = nullptr,
            CacheEntry *hPrev = nullptr, CacheEntry *hNext = nullptr)
    : key(kk)
    , data(dd)
    , dListPrev(dPrev)
    , dListNext(dNext)
    , hashMapPrev(hPrev)
    , hashMapNext(hNext)
    {}
    CacheEntry() // 用来构建表头
    : CacheEntry(0, 0)
    {}

    CacheEntry *insertDListBefore(CacheEntry *entry)
    {
        entry->dListNext = this;
        this->hashMapPrev = entry;
        return entry;
    }

    CacheEntry *insertDListAfter(CacheEntry *entry)
    {
        this->dListNext = entry->dListNext;
        if(this->dListNext)
            this->dListNext->dListPrev = this;
        entry->dListNext = this;
        this->dListPrev = entry;
        return this;
    }

    CacheEntry *insertHashMapBefore(CacheEntry *entry)
    {
        entry->hashMapNext = this;
        this->hashMapPrev = entry;
        return entry;
    }

    KeyType key;
    DataType data;

    CacheEntry *dListPrev;
    CacheEntry *dListNext;
    CacheEntry *hashMapPrev;
    CacheEntry *hashMapNext;
};

#endif
