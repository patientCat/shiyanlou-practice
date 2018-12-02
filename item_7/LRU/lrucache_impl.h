#ifndef _LRUCACHE_IMPL_H
#define _LRUCACHE_IMPL_H

typedef struct cacheEntryS{
    char key; // 数据key
    char data; // 数据data

    struct cacheEntryS *hashListPrev; // 缓存哈希表指针，指向哈希链表的前一个元素
    struct cacheEntryS *hashListNext; // 缓存哈希表指针，指向哈希链表的后一个元素

    struct cacheEntryS *lruListPrev; // 缓存双向链表指针，指向链表的前一个元素
    struct cacheEntryS *lruListNext; // 缓存双向链表指针，指向链表的后一个元素
}cacheEntryS;
typedef struct LRUCacheS{
    int cacheCapacity; // 缓存容量
    cacheEntryS **hashMap; // 缓存的哈希表

    cacheEntryS *lruListHead; // 缓存双向链表表头
    cacheEntryS *lruListTail; // 缓存双向链表表尾
    int lruListSize;    // 缓存双向链表节点个数
}LRUCacheS;

static cacheEntryS *newCacheEntry(char key, char data);
static void freeCacheEntry(cacheEntryS * entry);

int LRUCacheCreate(int capacity, void **lruCache);
int LRUCacheDestroy(void *lruCache);
int LRUCacheSet(void *lruCache, char key, char data);
char LRUCacheGet(void *lruCache, char key);
void LRUCachePrint(void *lruCache);

#endif // lrucache_imp.h
