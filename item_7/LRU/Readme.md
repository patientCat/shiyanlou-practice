# LRU缓存设计
## do while(0)的设计
## 数据结构
### 缓存表设计

```cpp
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
```
Entry项，缓存表项<br/>
采用key-data键值对的方式来存储数据。<br/>
因为缓存表中，我们既需要双向链表来存储数据。同时在查找数据时，也需要哈希表。所有首先需要双向链表指针。当
发生哈希冲突的时候，我们需要内建链表来存储发生冲突的元素。所以需要哈希表指针。<br/>

理解如上的数据结构是理解LRU核心思想的关键之一。即我们将数据即组织为了双向链表，又组织为了哈希表结构。


## 操作
简单来说，我们至少需要如下操作。<br/>
1. 创建容量为多少的缓存表。输入参数：LRUCache句柄 给定容量，因为需要设置句柄，所以需要传递二级指针
2. 删除缓存表。输入参数：LRUCache句柄 需要释放所有空间<br/>
3. 添加表项，
    1. 需要将根据key-data键值对建立一个表项
    2. 将这个表项插入到双向链表中。如果缓存已满，还需要删除访问时间最久的表项（因为我们双向链表存储的方式就是
    按照时间排序，所以就是删除末尾项，同时需要删除哈希表中的项）
    3. 插入到双向链表中，同时也插入到哈希表中。
4. 取出表项，唯一需要注意的是更新表项即可，在哈希表中的结构不需要修改，只需要修改双向链表即可。

对于创建缓存表项，我们在提供key-data对那里创建。<br/>
对于真正删除缓存表项，我们在这个旧表项被新表项淘汰时删除。<br/>
