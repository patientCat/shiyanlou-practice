#ifndef _hashMap_h_
#define _hashMap_h_

#include "lruentry.hh" 

using hashNode = CacheEntry;
class HashMap{
public:
    HashMap(int capacity)
    : _capacity(capacity)
    {
        _hashmap = new hashNode*[capacity];
        for(int i = 0; i < capacity; ++i)
            _hashmap[i] = nullptr;
    }

    hashNode * insert(hashNode *pnode)
    {
        hashNode * entry = _hashmap[hashkey(pnode->key, *this)];
        // 如果发生了哈希冲突
        if(nullptr != entry)
        {
            pnode->hashMapNext = entry;
            entry->hashMapPrev = pnode;
        }
        _hashmap[hashkey(pnode->key, *this)] = pnode;
        return pnode;
    }

    void remove(hashNode *pnode)
    {
        hashNode * entry = _hashmap[hashkey(pnode->key, *this)];
        while(entry->data != pnode->data)
        {
            entry = entry->hashMapNext;
        }

        if(entry->hashMapNext)
        {
            entry->hashMapNext->hashMapPrev = entry->hashMapPrev;
        }

        if(entry->hashMapPrev)
        {
            entry->hashMapPrev->hashMapNext = entry->hashMapNext;
        }
        else{
            _hashmap[hashkey(pnode->key, *this)] = entry->hashMapNext;
        }
    }

    hashNode * find(KeyType key)
    {
        return _hashmap[hashkey(key, *this)];
    }
    int capacity() const { return _capacity;    }

    static KeyType hashkey(KeyType key, HashMap &hm)
    {
        return key % hm.capacity();
    }
private:
    hashNode **_hashmap;
    int _capacity;
};
#endif
