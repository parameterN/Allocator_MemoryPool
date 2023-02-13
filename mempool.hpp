#ifndef _MEMPOOL_H
#define _MEMPOOL_H

#include<stdlib.h>

#define BLOCK_SIZE (sizeof(void*)*10) /* 80 in 64 bit operating system */
#define POOL_SIZE 1000000 /* maximal pool size for allocating */
#define POOL_WIDTH 100000 /* the maximal blocks that the free_list can manage */

/* the struct to maintain a large consecutive block of memory */
typedef struct node
{ 
    void * Head; /* void pointer, point the address of the head of the space */
    struct node * Next; /* point to the next space */
    node()
    {
        Head = ::operator new(BLOCK_SIZE * POOL_SIZE);
        Next = NULL;
    }
    ~node(){::operator delete(Head);}   /* dtor of the struct */
}Pool_Head;
typedef Pool_Head * pPool_Head;
class MemPool
{
private:
    pPool_Head Undivided_Pool; /* a big consecutive block of memory */
    void * Trace;   /* pointing to the unused part of the current consecutive block */
    pPool_Head Trace_Head;  /* pointing to the head of the current consecutive block */
    int Free_Num;   /* the number of unused blocks */
    void * Divided_Pool[POOL_WIDTH];    /* the free_list to store released spaces with different number of blocks(the bin) */
public:
    MemPool()/* constructor of each memory pool */
    {
        Undivided_Pool = new Pool_Head; 
        Trace_Head = Undivided_Pool;    /* initializing the Trace_Head */
        Trace = Undivided_Pool->Head;   /* void* trace will record the address of the current unused space */
        Free_Num = POOL_SIZE;           /* Free_Num is initialized as the total number of spaces of the memory pool */
        for(int i = 0; i <  POOL_WIDTH; i++)Divided_Pool[i] = NULL; /* each bin will be initialized to NULL */
    }

    ~MemPool()
    {
        while(Undivided_Pool != NULL) /* destruct every consecutive block */
        {
            Trace_Head = Undivided_Pool->Next;
            delete Undivided_Pool;
            Undivided_Pool = Trace_Head;
        } 
    }

    inline int Block_Num(size_t size) /* determine the number of blocks to be allocated */
    {
        if(size%BLOCK_SIZE==0)return size/BLOCK_SIZE;
        else return size/BLOCK_SIZE + 1; 
    }

    void * Allocate(size_t size)
    {
        int block_num = Block_Num(size); /* get numbers of blocks we will use */
        if(block_num > POOL_SIZE/10){return ::operator new(size);} /* if the number of block is too large, we will
                                                                    use the default allocting approaches provided by the operating system */
        if(block_num <= POOL_WIDTH && Divided_Pool[block_num-1]!=NULL) /* if there are blocks of suitable size in the bin, we will use these blocks first */
        {
            void * ptr = Divided_Pool[block_num-1]; /* take the block out of the bin */
            Divided_Pool[block_num-1] =
            static_cast<void *>(*(static_cast<int **>(Divided_Pool[block_num-1])));
            /* move the pointer to the first eight bytes of the space it pointed,
             which has the function similar to ptr = ptr->Next */
            return ptr;
        }
        if(block_num > Free_Num)    /* if the unused space is not enough */
        {
            Trace_Head->Next = new Pool_Head; /* develop a new large consecutive block */
            Trace_Head = Trace_Head->Next;  /* set the Trace_Head to the new head */
            /*divide*/
            Trace = Trace_Head->Head; /* initialize Trace to the beginning of the new space */
            Free_Num = POOL_SIZE;     /* initialize the Free_Num to the maximal value */
        }
        /* other situations */
            void * ptr = Trace; /* ptr will point to the place that Trace pointed */
            Free_Num -= block_num; /* update Free_Num */
            char * temp = static_cast<char *>(Trace);
            temp += block_num*BLOCK_SIZE;  /* move Trace forawrd a spefic number of bytes*/
            Trace = static_cast<void *>(temp);
            return ptr;
            /* this part means to cut a piece of the memory in the consecutive block and update Trace accordingly */
    }

    void Deallocate(void * ptr,size_t size)
    {
        int block_num = Block_Num(size);
        // if(block_num > POOL_SIZE/10){::operator delete(ptr);return;}
        if(block_num > POOL_WIDTH)return;//Unable to manage
        int ** p = static_cast<int **>(ptr);/* force to turn to int** type */
        *p = static_cast<int *>(Divided_Pool[block_num-1]);
        Divided_Pool[block_num-1] = static_cast<void *>(p); /* insert the space into the free_list for recycling */
    }
};

#endif