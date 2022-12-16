#include <iostream>
#include <memory>
#include<vector>
#include "allocator.hpp"
using namespace std;

int main(int argc, char const *argv[])
{
    vector<int,MyAllocator<int>> v1(5);
    for(int i = 0; i < 5; i++){cout << v1[i] << " ";}
    cout << endl; 
    cout << v1.capacity() << endl;
    for(int i = 0; i < 5; i++)
    {v1[i] = i;} 
    for(int i = 0; i < 5; i++){cout << v1[i] << " ";}
    cout << endl; 
    return 0;
}
