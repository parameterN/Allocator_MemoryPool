#ifndef _MEMPOOL_H
#define _MEMPOOL_H

#include <iostream>

#define BLOCK_SIZE (sizeof(void*)*50)
#define POOL_SIZE 1000000
#define POOL_WIDTH 1000

typedef struct node
{
    void * Head;
    struct node * Next;
    node()
    {
        Head = ::operator new(BLOCK_SIZE * POOL_SIZE);
        // if(Head == NULL) 
        // std::cout << "Out of memory" << std::endl;
        Next = NULL;
    }
    ~node(){::operator delete(Head);}
}Pool_Head;
typedef Pool_Head * pPool_Head;
class MemPool
{
private:
    size_t block_size;
    pPool_Head Undivided_Pool;
    void * Trace;
    pPool_Head Trace_Head;
    int Free_Num;
    void * Divided_Pool[POOL_WIDTH];
public:
    MemPool()
    {
        block_size = BLOCK_SIZE;
        Undivided_Pool = new Pool_Head;
        Trace_Head = Undivided_Pool;
        Trace = Undivided_Pool->Head;
        Free_Num = POOL_SIZE;
        for(int i = 0; i <  POOL_WIDTH; i++)Divided_Pool[i] = NULL;
    }

    ~MemPool()
    {
        while(Undivided_Pool != NULL)
        {
            Trace_Head = Undivided_Pool->Next;
            delete Undivided_Pool;
            Undivided_Pool = Trace_Head;
        } 
    }

    inline int Block_Num(size_t size)
    {
        if(size%BLOCK_SIZE==0)return size/BLOCK_SIZE;
        else return size/BLOCK_SIZE + 1; 
    }

    void * Allocate(size_t size)
    {
        int block_num = Block_Num(size);
        if(block_num > POOL_SIZE/10){return ::operator new(size);}
        if(block_num <= POOL_WIDTH && Divided_Pool[block_num-1]!=NULL)
        {
            void * ptr = Divided_Pool[block_num-1];
            Divided_Pool[block_num-1] =
            static_cast<void *>(*(static_cast<int **>(Divided_Pool[block_num-1])));
            return ptr;
        }
        if(block_num > Free_Num)
        {
            Trace_Head->Next = new Pool_Head;
            Trace_Head = Trace_Head->Next;
            /*divide*/
            Trace = Trace_Head->Head;
            Free_Num = POOL_SIZE;
        }
            void * ptr = Trace;
            Free_Num -= block_num;
            char * temp = static_cast<char *>(Trace);
            temp += block_num*BLOCK_SIZE;
            Trace = static_cast<void *>(temp);
            return ptr;

        // return ::operator new(size);
    }

    void Deallocate(void * ptr,size_t size)
    {
        int block_num = Block_Num(size);
        if(block_num > POOL_SIZE/10){::operator delete(ptr);return;}
        if(block_num > POOL_WIDTH)return;//Unable to manage
        int ** p = static_cast<int **>(ptr);
        *p = static_cast<int *>(Divided_Pool[block_num-1]);
        Divided_Pool[block_num-1] = static_cast<void *>(p);
        // ::operator delete(ptr);
    }
};

#endif