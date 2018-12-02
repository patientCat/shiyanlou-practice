//c++ program
//design by >^.^<
//@date    2018-10-26 19:12:41

#include "lrucache_impl.h"
#include "lrucache.h"

#include <stdio.h>
#include <stdlib.h>

static void removeFromList(LRUCacheS * cache, cacheEntryS *entry);
// 创建一个缓存单元
static cacheEntryS *newCacheEntry(char key, char data)
{
    cacheEntryS *entry = NULL;
    if(NULL == (entry = (cacheEntryS *)calloc(1, sizeof(cacheEntryS))))
    {
        perror("calloc");
        exit(-1);
    }
    entry->key = key;
    entry->data = data;
    return entry;
}


// 释放一个缓存单元
static void freeCacheEntry(cacheEntryS * entry)
{
    if(NULL == entry)
        return ;
    free(entry);
}


// 双向链表操作
/*
 * 将节点插入到双向链表中
 */
static cacheEntryS * insertToListHead(LRUCacheS * cache, cacheEntryS *entry)
{
    cacheEntryS * removedEntry = NULL;
    if(++cache->lruListSize > cache->cacheCapacity)
    {
        // 如果缓存满了即链表当前节点已经等于缓存容量，需要先删除链表表尾节点，即淘汰最久没有被访问
        // 到的缓存数据单元
        removedEntry = cache->lruListTail;
        removeFromList(cache, cache->lruListTail);
    }

    if(cache->lruListHead == NULL && cache->lruListTail == NULL)
    {   
        // 如果当前链表为空链表
        cache->lruListHead = cache->lruListTail = entry;
    }
    else{
        // 如果当前链表非空，插入表头
        entry->lruListNext = cache->lruListHead;
        cache->lruListHead->lruListPrev = entry;
        cache->lruListHead = entry;
        entry->lruListPrev = NULL;
    }
    return removedEntry;
}

/*
 * 将节点从链表中删除
 */
static void removeFromList(LRUCacheS * cache, cacheEntryS *entry)
{
    // 如果链表为空
    if(0 == cache->lruListSize)
    {
        // handle_error
        return ;
    }
    if(1 == cache->lruListSize) // 如果只有一个节点
    {
        cache->lruListHead = cache->lruListTail = NULL;
    }
    else if(entry == cache->lruListHead)
    {
        cache->lruListHead = cache->lruListHead->lruListNext;
        cache->lruListHead->lruListPrev = NULL;
    }
    else if(entry == cache->lruListTail)
    {
        cache->lruListTail = cache->lruListTail->lruListPrev;
        cache->lruListTail->lruListNext = NULL;
    }
    else{
        entry->lruListPrev->lruListNext = entry->lruListNext;
        entry->lruListNext->lruListPrev = entry->lruListPrev;
    }
    --cache->lruListSize;
}


/*
 * 释放整个链表
 */
static void freeList(LRUCacheS *cache)
{
    // 如果整个链表为空
    if(0 == cache->lruListSize)
        return ;
    cacheEntryS *scan = cache->lruListHead;
    cacheEntryS *pre = NULL;
    while(scan)
    {
        pre = scan;
        scan = scan->lruListNext;
        freeCacheEntry(pre);
    }
    cache->lruListSize = 0;
    cache->lruListHead = cache->lruListTail = NULL;
}

/*
 * 辅助性接口，用于保证最近访问的节点总是位于链表表头
 * 将entry更新到表头
 */
static void updateLRUList(LRUCacheS * cache, cacheEntryS *entry)
{
    // 将节点从链表中删除
    removeFromList(cache, entry);
    insertToListHead(cache, entry);
}

/* 哈希函数 */
static int hashKey(LRUCacheS *cache, int key)
{
    return key % cache->cacheCapacity;
}

/*
 * 从哈希表获取缓存单元
 */
static cacheEntryS * getValueFromHashMap(LRUCacheS *cache, int key)
{
    // 根据哈希函数定位数据在哪个槽内
    cacheEntryS *entry = cache->hashMap[hashKey(cache, key)];
    if(!entry)  return NULL;

    // 遍历查询槽内链表
    while(entry)
    {
        if(entry->key == key)
            break;
        entry = entry->hashListNext;
    }
    return entry;
}

/*
 * 向哈希表中插入缓存单元
 */
static void insertEntryToHashMap(LRUCacheS *cache, cacheEntryS *entry)
{
    // 使用哈希函数定位数据存放在哪个槽
    cacheEntryS *n = cache->hashMap[hashKey(cache, entry->key)];

    // 如果发生哈希冲突, 将当前entry插为表头
    if(n != NULL)
    {
        entry->hashListNext = n;
        n->hashListPrev = entry;
    }
    cache->hashMap[hashKey(cache, entry->key)] = entry;
}

/*
 * 从哈希表删除缓存单元
 */
static void removeEntryFromHashMap(LRUCacheS *cache, cacheEntryS *entry)
{
    // 无需做任何操作的情况
    if(entry == NULL || NULL == cache || NULL == cache->hashMap)
        return ; 
    // 定位
    cacheEntryS *n = cache->hashMap[hashKey(cache, entry->key)];
    while(n)
    {
        if(n->key == entry->key)
        {
            if(n == cache->hashMap[hashKey(cache, entry->key)])
            {
                cache->hashMap[hashKey(cache, entry->key)] = n->hashListNext;
                if(cache->hashMap[hashKey(cache, entry->key)])
                    cache->hashMap[hashKey(cache, entry->key)]->hashListPrev = NULL;
            }
            else{
                n->hashListPrev->hashListNext = n->hashListNext;
                if(n->hashListNext)
                    n->hashListNext->hashListPrev = n->hashListPrev;
            }
            return;
        }
        n = n->hashListNext;
    }
}

/*
 * 创建LRU缓存
 * capacity 输入参数，指定新建缓存的数据容量
 * lruCache 输出参数，新建缓存句柄
 * 返回值： 成功返回0，失败返回-1
 */
int LRUCacheCreate(int capacity, void **lruCache)
{
    LRUCacheS *cache = NULL;
    if(NULL == (cache = (LRUCacheS *)calloc(1, sizeof(LRUCacheS))))
    {
        perror("calloc");
        exit(-1);
    }
    cache->cacheCapacity = capacity;
    cache->hashMap = (cacheEntryS **)calloc(capacity, sizeof(cacheEntryS*));
    if(NULL == cache->hashMap)
    {
        perror("calloc");
        exit(-1);
    }
    *lruCache = cache;
    return 0;
}


/*
 * 销毁LRU缓存
 * lruCache 输入参数，
 * 返回值： 成功返回0，失败返回-1
 */
int LRUCacheDestroy(void *lruCache)
{
    LRUCacheS *cache = (LRUCacheS *) lruCache;
    if(NULL == cache)
        return 0;
    //Todo : free hashmap
    //Todo : free linklist
    free(cache);
    return 0;
}

/*
 * 将数据放入LRU缓存中
 * lruCache 输入参数，缓存句柄
 * key 输入参数，数据索引
 * data 输入参数，数据内容
 * 返回值： 成功返回0，失败返回-1
 */
int LRUCacheSet(void *lruCache, char key, char data)
{
    LRUCacheS *cache = (LRUCacheS *)lruCache;
    
    // 先查看缓存表中是否有数据
    cacheEntryS *entry = getValueFromHashMap(cache, key);
    if(NULL != entry)
    {
        updateLRUList(cache, entry);
    }
    else{
        // 数据不在缓存中
        // 新建缓存单元
        entry = newCacheEntry(key, data);

        // 将新建缓存单元插入链表表头
        cacheEntryS * removedEntry = insertToListHead(cache, entry);
        if(NULL != removedEntry)
        {
            // 新建缓存单元发生缓存满了的情况，需要淘汰访问时间最久的entry，已经从双向链表中删除
            // 需要从hashMap中删除
            removeEntryFromHashMap(cache, removedEntry);
            freeCacheEntry(removedEntry);
        }
        // 将新建缓存单元插入哈希表
        insertEntryToHashMap(cache, entry);
    }
    return 0;
}

/* 
 * 从缓存中获取数据
 * lruCache 输入参数，缓存句柄
 * key 输入参数，数据索引
 * 返回值： 存在key，返回相关值，不存在返回0，即成功
 */
char LRUCacheGet(void *lruCache, char key)
{
    LRUCacheS *cache = (LRUCacheS *)lruCache;
    cacheEntryS * entry = cache->hashMap[hashKey(cache, key)];
    if(NULL != entry)
    {
        // 缓存中存在该数据，更新至链表表头
        updateLRUList(cache, entry);
        return entry->data;
    }
    else{
        // 缓存中不存在相关数据
        return 0;
    }
}


/*
 * 打印缓存中的数据，按照访问时间从新到旧输出
 * lruCache 输入参数缓存句柄
 */
void LRUCachePrint(void *lruCache)
{
    LRUCacheS *cache = (LRUCacheS *)lruCache;
    if(!cache->lruListSize || !cache)
        return ;
    fprintf(stdout, "\n>>>>>>>>>>\n");
    fprintf(stdout, "cache  (key  data):\n");
    cacheEntryS *entry = cache->lruListHead;
    while(entry) {
        fprintf(stdout, "(%c, %c) ", entry->key, entry->data);
        entry = entry->lruListNext;
    }
    fprintf(stdout, "\n<<<<<<<<<<\n\n");
}
