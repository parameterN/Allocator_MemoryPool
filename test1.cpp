#include <iostream>
#include "allocator.hpp"
#include <time.h>
#define TEST_TIME 1000000

clock_t start,stop; 
clock_t ticks; //The duration ticks
double total;  //The total run time
MemPool pool;
template<class T>
MemPool* myAllocator<T>::mempool = &pool;

int main()
{
    std::allocator<int> a1;
    myAllocator<int> a2;
    start = clock();
    for (int i = 0; i < TEST_TIME; i++)
    {
        int *p = a1.allocate(i%1000+1);
        a1.deallocate(p,i%1000+1);
        // int *p = a1.allocate(1);
        // a1.deallocate(p,1);
    }
    stop = clock();
    ticks = stop - start; //Calculate the duration
    total = ((double)(stop - start))/CLK_TCK;
    std::cout << "std: " << total << std::endl;

    start = clock();
    for (int i = 0; i < TEST_TIME; i++)
    {
        int *p = a2.allocate(i%1000+1);
        a2.deallocate(p, i%1000+1);
        // int *p = a2.allocate(1);
        // a2.deallocate(p, 1);
    }
    stop = clock();
    ticks = stop - start; //Calculate the duration
    total = ((double)(stop - start))/CLK_TCK;
    std::cout << "mempool: " << total << std::endl;
    return 0;
}