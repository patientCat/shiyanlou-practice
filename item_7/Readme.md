### LRU 缓存实现
底层实现为一个dList和hashMap，和平常使用的数据结构不同的是，这俩个底层结构是一种复合，或者说内涵的方式使用的。
也可以说完成了数据结构的真正思想，即不管底层的实现如何，在使用者看来，其就是某种数据结构的方式。

### 实现
总的来说，我们首先需要一个lruCache的表用来组织整体的数据结构。其次最重要的就是缓存表的表项问题。<br/>
因为我们既希望通过dList访问，也希望使用hashMap来访问，所以表项需要俩套指针。<br/>
```cpp
typedef struct cacheEntryS{
    KeyType key; // 数据key
    DataType data; // 数据data

    struct cacheEntryS *hashListPrev; // 缓存哈希表指针，指向哈希链表的前一个元素
    struct cacheEntryS *hashListNext; // 缓存哈希表指针，指向哈希链表的后一个元素

    struct cacheEntryS *lruListPrev; // 缓存双向链表指针，指向链表的前一个元素
    struct cacheEntryS *lruListNext; // 缓存双向链表指针，指向链表的后一个元素
}cacheEntryS;
```

### 数据结构的实现
数据结构的实现没什么好说的，其中C实现dList的方式采用了无头结点的方式实现。C++实现dList采用了有头结点的方式实现。<br/>
和平常所不同的是，对于所有的元素，我们均采用指针的方式，即我们的数据结构只起组织作用。对于具体的表项删除和创建
工作，全部交给缓存表类来做。<br/>
简单描述下dList的几个操作的重点。<br/>
insertAsHead() 头插法，注意是否使用头结点还是有些区别的。<br/>
1. 不使用头结点的情况下, 分为俩种情况。
    * 不插入节点时，`_head = _tail = nullptr;`
    * 插入第一个节点时，`_head = _tail = newnode;`
    * 插入之后的节点时，`newnode->next = _head; _head = newnode;`
2. 使用头结点的情况下, 分为俩种情况。
    * 不插入节点时，`_tail = _head;` 此时是有头结点。
    * 插入第一个节点时，`_head->next = newnode; _tail = newnode;`
    * 插入之后的节点时，`newnode->next = _head->next; _head->next = newnode;`
3. 使用头尾节点的情况下，
    * 不插入节点时，`_head->next = _tail;`
    * 插入任何位置的节点, `newnode->next = _head->next; _head->next = newnode;`

removeFromList() 删除节点，或者removeTail()删除尾节点。<br/>
主要考虑的有这么三种情况。<br/>
1. 删除的节点是表中最后一个节点。
2. 是否是头结点或者尾节点。
3. 其他节点。

hashMap的hashKey采取了最简单的方式即取余来实现，如果发生hash冲突，解决的方式也仅仅是在冲突位置下挂链表。<br/>
