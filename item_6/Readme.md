### 多线程实现归并排序
归并排序中的俩个mergeSort在单个线程执行时，效率较慢。需要等一个执行完，才可以等另一个。
所以采取多线程的方式进行分别执行。但是这样创建线程的时候比较浪费时间。<br/>
mergeSort_inpthread<br/>
为多线程完成归并排序，但缺点在于需要创建线程。这里的开销比较大。在面对大数量的情况下，不是一种切实
可行的方法, 仅仅是提供理论上的联系。

### 多线程排序
另一种方法是，假设现在有N个线程，此时使用N个线程分别对数组的子序列进行快排。然后由主线程归并他们。<br/>
这算是一种经典的外部排序方法，首先将大数据根据线程数据K进行划分，每个线程得到step长的数据。最后剩余的部分交给主线程
进行处理。子线程分别进行 采取快排的方式将其排序完毕。然后交给主线程归并。对于等长数组经过Kmerge方法。其核心为采取
每一路的首元素建一个 K个大小的小根堆。每次弹出堆顶元素，然后根据堆顶元素原路的下一个元素，当某路元素用完时，加入
无穷大。循环结束可以采取当堆顶取到无限大或者待排元素已经完毕。我用的是后者。然后对于不等长的元素，进行一次二路归并
即可。<br/>
我这里纯粹是为了练习这样写的，事实上一遍K路归并既可以完成所以数组的归并。我这里只是为了再练习下二路归并。采用C++
更好写一点。最起码不用写这么多轮子。<br/>
