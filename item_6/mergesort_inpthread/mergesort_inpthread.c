//c++ program
//design by >^.^<
//@date    2018-10-23 15:44:51

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

typedef struct _Vec{
    int *array;
    int low;
    int high;
}Vec;

void merge(int *array, int low, int mid, int high)
{
    int lenB = mid - low;
    int lenC = high - mid;
    int * A = array + low;
    int * B = (int *)calloc(lenB, sizeof(int));
    for(int i = 0; i < lenB; ++i)
        B[i]= A[i];
    int * C = array + mid;
    for(int i = 0, j = 0, k = 0; j < lenB; )
    {
        if(lenC <= k || B[j] <= C[k])
            A[i++] = B[j++];
        if(k < lenC && C[k] < B[j])
            A[i++] = C[k++];
    }
    free(B);
}

void *mergeSort(void *p)
{
    Vec *arr = (Vec *)p;
    if(arr->high  <= arr->low + 1)
        return NULL;
    pthread_t pth_1, pth_2; 
    int mid = (arr->high + arr->low) >> 1;
    Vec vec_1, vec_2;
    vec_1.array = arr->array;
    vec_1.low = arr->low; vec_1.high = mid;
    pthread_create(&pth_1, NULL, mergeSort, (void *)&vec_1);
    vec_2.array = arr->array;
    vec_2.low = mid; vec_2.high = arr->high;
    pthread_create(&pth_2, NULL, mergeSort, (void *)&vec_2);
    pthread_join(pth_1, NULL);
    pthread_join(pth_2, NULL);
    merge(arr->array, arr->low, mid, arr->high);
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

void readArray(int fd, int * array, int size)
{
    char word[11] = {0};
    for(int i = 0; i < size; ++i)
    {
        readWord(fd, word, 11);
        array[i] = tonumber(word);
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
    Vec vec;
    vec.low = 0;
    vec.high = length;
    vec.array = (int *)calloc(length, sizeof(int));
    readArray(fd, vec.array, length);
    for(int i = 0; i < length; ++i)
        printf("%3d", vec.array[i]);
    printf("\n");
    pthread_t ms;
    pthread_create(&ms, NULL, mergeSort, (void *)&vec);
    pthread_join(ms, NULL);
    for(int i = 0; i < length; ++i)
        printf("%3d", vec.array[i]);
    printf("\n");
    return 0;
}
