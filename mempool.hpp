#include <iostream>
#include <memory.h>
#include<vector>
using namespace std;

#define BLOCK_SIZE (sizeof(void*))
#define POOL_SIZE 10000
#define POOL_WIDTH 10
class MemPool
{
private:
    size_t block_size;
    void * Undivided_Pool;
    void * Trace;
    int Free_Num;
    void * Divided_Pool[POOL_WIDTH];
public:
    MemPool()
    {
        block_size = BLOCK_SIZE;
        Undivided_Pool = ::operator new(BLOCK_SIZE * POOL_SIZE);
        Trace = Undivided_Pool;
        Free_Num = POOL_SIZE;
        for(int i = 0; i <  POOL_WIDTH; i++)Divided_Pool[i] = NULL;
        cout << "Create MemPool" << endl;
    }

    ~MemPool()
    {
        cout << "Delete MemPool " << this << endl;
        ::operator delete(Undivided_Pool);
    }

    int Block_Num(size_t size)
    {
        if(size%BLOCK_SIZE==0)return size/BLOCK_SIZE;
        else return size/BLOCK_SIZE + 1; 
    }

    void * Allocate(size_t size)
    {
        int block_num = Block_Num(size);
        if(block_num >= POOL_WIDTH){return ::operator new(size);}
        //if(block_num > Free_Num){cout << "Memory pool is exhausted!" << endl;return NULL;}
        if(Divided_Pool[block_num-1]!=NULL)
        {
            void * ptr = Divided_Pool[block_num-1];
            Divided_Pool[block_num-1] =
            static_cast<void *>(*(static_cast<int **>(Divided_Pool[block_num-1])));
            return ptr;
        }
        else
        {
            void * ptr = Trace;
            Free_Num -= block_num;
            char * temp = static_cast<char *>(Trace);
            temp += block_num*BLOCK_SIZE;
            Trace = static_cast<void *>(temp);
            return ptr;
        }
        return ::operator new(size);
    }

    void Deallocate(void * ptr,size_t size)
    {
        int block_num = Block_Num(size);
        if(block_num >= POOL_WIDTH){::operator delete(ptr);return;}
        int ** p = static_cast<int **>(ptr);
        *p = static_cast<int *>(Divided_Pool[block_num-1]);
        Divided_Pool[block_num-1] = static_cast<void *>(p);
    }
};
