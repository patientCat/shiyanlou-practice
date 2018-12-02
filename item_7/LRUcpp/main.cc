//c++ program
//design by >^.^<
//@date    2018-11-05 10:29:00
#include "lru.hh"

void test1()
{
    fprintf(stdout, "=========================\n");
    fprintf(stdout, "In testcase1....\n");
    fprintf(stdout, "=========================\n");

    Cache lruCache(5);
    lruCache.LRUCacheSet(1, 'A'); 
    lruCache.LRUCacheSet(2, 'B'); 
    lruCache.LRUCacheSet(3, 'C'); 
    lruCache.LRUCacheSet(4, 'D'); 
    lruCache.LRUCacheSet(5, 'E'); 
    lruCache.print();
    fprintf(stdout, "\n should be E D C B A \n");
    fprintf(stdout, "after update\n");
    lruCache.LRUCacheSet(6, 'F'); 
    lruCache.print();
    fprintf(stdout, "\n should be F E D C B \n");
    lruCache.LRUCacheGet(2); 
    lruCache.LRUCacheGet(3); 
    lruCache.print();
    fprintf(stdout, "\n should be C B F E D \n");
}

int main()
{
    test1();
    return 0;
}
