#ifndef _LRUCACHE_H_
#define _LRUCACHE_H_

int LRUCacheCreate(int capacity, void **lruCache);
int LRUCacheDestroy(void *lruCache);
int LRUCacheSet(void *lruCache, char key, char data);
char LRUCacheGet(void *lruCache, char key);
void LRUCachePrint(void *lruCache);

#endif // lurcache_h
