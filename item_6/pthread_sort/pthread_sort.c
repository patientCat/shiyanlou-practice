//c++ program
//design by >^.^<
//@date    2018-10-23 15:44:51

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define MAXPTHREADS 4
#define swap(a, b) {    int temp = (a);\
                        (a) = (b);\
                        (b) = temp; }
typedef struct _Element{
    int data;
    int next;
    int index;
}Element;

typedef struct _Vec{
    Element *array;
    int low;
    int high;
}Vec;

void percolateDown(Element *arr, int pos, int size);
void merge2(Element *arr1, int n1, Element *arr2, int n2)
{
    int i = 0, j = 0, k = 0;
    Element *tmp = (Element *)calloc(n1, sizeof(Element));
    for(int i = 0; i < n1; ++i)
        tmp[i] = arr1[i];
    while(j < n1 && k < n2)
    {
        if(tmp[j].data <= arr2[k].data)
            arr1[i++].data = tmp[j++].data;
        else
            arr1[i++].data = arr2[k++].data;
    }
    while(j < n1)
        arr1[i++].data = tmp[j++].data;
    while(k < n2)
        arr1[i++].data = arr2[k++].data;
    free(tmp);
}
void makeHeap(Element *arr, int n)
{
    int lastP = n / 2 - 1;
    for(int i = lastP; i > -1 ; --i)
        percolateDown(arr, i, n);
}
void percolateDown(Element *arr, int pos, int size)
{
    while(2 * pos + 1 < size)
    {
        int lchild = 2 * pos + 1;
        int rchild = lchild + 1;
        int smallchild = lchild;
        if(rchild < size && arr[rchild].data < arr[lchild].data)
            ++smallchild;
        if(arr[pos].data <= arr[smallchild].data)
            break;
        Element tmp;
        tmp = arr[pos];
        arr[pos] = arr[smallchild];
        arr[smallchild] = tmp;
        pos = smallchild;
    }
}
void kmerge(Element *arr, int size, int step) // 合并k个有序数组，每个数组均为step
{
    Element *ret = (Element *)calloc(size, sizeof(Element));
    int k = size / step ;
    Element **datas = (Element **)calloc(k, sizeof(Element *));
    for(int i = 0; i < k; ++i)
    {
        datas[i] = &arr[i * step];
        for(int j = 0; j < step; ++j)
        {
            datas[i][j].next = j + 1;
            datas[i][j].index = i;
        }
    }
    Element *pq = (Element *)calloc(k, sizeof(Element));
    for(int i = 0; i < k; ++i)
        pq[i] = *datas[i];
    makeHeap(pq, k);
    for(int i = 0; i < size;)
    {
        ret[i++].data = pq[0].data;
        if(pq[0].next == step)
            pq[0].data = INT_MAX;
        else{
            int index = pq[0].index;
            int next = pq[0].next;
            pq[0] = datas[index][next];
        }
        percolateDown(pq, 0, k);   
    }
    for(int i = 0; i < size; ++i)
        arr[i] = ret[i];
}
int partition(Element *arr, int low, int high)
{
    --high;
    int candidate = arr[low].data;
    while(low < high)
    {
        while(low < high && arr[high].data >= candidate)
            --high;
        arr[low].data = arr[high].data;
        while(low < high && arr[low].data <= candidate)
            ++low;
        arr[high].data = arr[low].data;
    }
    arr[low].data = candidate;
    return low;
}

void quickSort(Element *arr, int low, int high)
{
    if(low + 1 < high)
    {
    int pivot = partition(arr, low, high);
    quickSort(arr, low, pivot);
    quickSort(arr, pivot + 1, high);
    }
}

void *pthread_routine(void *p)
{
    Vec *vec = (Vec *)p;
    quickSort(vec->array, vec->low, vec->high);
    return NULL;
}
int readWord(int fd, char *word, int size)
{
    bzero(word, size);
    int i = 0;
    int ret = 0;
    char c;
    while((ret = read(fd, &c, 1)))
    {
        if(c == ' ' || c == '\n')
            break;
         word[i++] = c;
    }
    return ret;
}

int tonumber(char *str)
{
    int ret = 0;
    int num = 0;
    for(size_t i = 0; i < strlen(str); ++i)
    {
        num = str[i] - '0';
        ret = ret * 10 + num;
    }
    return ret;
}

int readLength(int fd)
{
    char word[11] = {0};
    readWord(fd, word, 11);
    int num = tonumber(word);
    return num;
}

void readArray(int fd, Element * array, int size)
{
    char word[11] = {0};
    for(int i = 0; i < size; ++i)
    {
        readWord(fd, word, 11);
        array[i].data = tonumber(word);
    }
}

int main(int argc, char *argv[])
{
    if(argc < 1 || argc > 2)
    {
        printf("error\n");
        exit(-1);
    }
    const char * filename  = argv[1];
    int fd = open(filename, O_RDONLY);
    if(-1 == fd)
        perror("open");
    // 从文件中读取数据
    int length = readLength(fd);
    Element *array = (Element *)calloc(length, sizeof(Element));
    readArray(fd, array, length);
    for(int i = 0; i < length; ++i)
        printf("%3d", array[i].data);
    printf("\n");
    pthread_t pths[MAXPTHREADS];
    Vec vecs[MAXPTHREADS];
    int step = length / MAXPTHREADS;
    for(int i = 0; i < MAXPTHREADS; ++i)
    {
        vecs[i].array = array;
        vecs[i].low = i * step;
        vecs[i].high = (i + 1) * step;
        pthread_create(&pths[i], NULL, pthread_routine, (void *)&vecs[i]);
    }
    quickSort(array, vecs[MAXPTHREADS - 1].high, length);
    for(int i = 0; i < MAXPTHREADS; ++i)
    {
        pthread_join(pths[i], NULL);
    }
    // 未经过K路merge
    printf("before kmerge\n");
    for(int i = 0; i < length; ++i)
        printf("%3d", array[i].data);
    printf("\n");
    // K路merge
    // 这个算法如果在C语言的环境下，以数组的方式实现起来有点难度。
    printf("经过K路merge后\n");
    kmerge(array, step * MAXPTHREADS, step);
    for(int i = 0; i < length; ++i)
        printf("%3d", array[i].data);
    printf("\n");
    printf("再经过一次简单的2路合并\n");
    merge2(array, 12, array + 12, 3);
    for(int i = 0; i < length; ++i)
        printf("%3d", array[i].data);
    printf("\n");
    return 0;
}
