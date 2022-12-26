#include <iostream>
#include "allocator.hpp"
#include <random>
#include <time.h>
#include <vector>
// include header of your allocator here
template<class T>
using MyAllocator = myAllocator<T>; // replace the std::allocator with your allocator
template<class T>
using Allocator = std::allocator<T>; // replace the std::allocator with your allocator
using Point2D = std::pair<int, int>;

const int numsize = 10000;
const int TestSize = 10000;
const int PickSize = 1000;

clock_t start,stop; 
clock_t ticks; //The duration ticks
double total;  //The total run time
MemPool pool;
template<class T>
MemPool* myAllocator<T>::mempool = &pool;

int main()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, numsize);

  start = clock();

  // vector creation
  using IntVec = std::vector<int, MyAllocator<int>>;
  std::vector<IntVec, MyAllocator<IntVec>> vecints(TestSize);
  for (int i = 0; i < TestSize; i++)
    vecints[i].resize(dis(gen));

  using PointVec = std::vector<Point2D, MyAllocator<Point2D>>;
  std::vector<PointVec, MyAllocator<PointVec>> vecpts(TestSize);
  for (int i = 0; i < TestSize; i++)
    vecpts[i].resize(dis(gen));

  // vector resize
  for (int i = 0; i < PickSize; i++)
  {
    int idx = dis(gen) - 1;
    int size = dis(gen);
    vecints[idx].resize(size);
    vecpts[idx].resize(size);
  }

  // vector element assignment
  {
    int val = 10;
    int idx1 = dis(gen) - 1;
    int idx2 = vecints[idx1].size() / 2;
    vecints[idx1][idx2] = val;
    if (vecints[idx1][idx2] == val)
      std::cout << "correct assignment in vecints: " << idx1 << std::endl;
    else
      std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
  }
  {
    Point2D val(11, 15);
    int idx1 = dis(gen) - 1;
    int idx2 = vecpts[idx1].size() / 2;
    vecpts[idx1][idx2] = val;
    if (vecpts[idx1][idx2] == val)
      std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
    else
      std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
  }

  stop = clock();
  ticks = stop - start; //Calculate the duration
  total = ((double)(stop - start))/CLK_TCK;
  std::cout << "mempool: " << total << std::endl;
  
  //-----------------------------------------------------------------
  start = clock();

  // vector creation
  using IntVec2 = std::vector<int, Allocator<int>>;
  std::vector<IntVec2, Allocator<IntVec2>> vecints2(TestSize);
  for (int i = 0; i < TestSize; i++)
    vecints2[i].resize(dis(gen));

  using PointVec2 = std::vector<Point2D, Allocator<Point2D>>;
  std::vector<PointVec2, Allocator<PointVec2>> vecpts2(TestSize);
  for (int i = 0; i < TestSize; i++)
    vecpts2[i].resize(dis(gen));

  // vector resize
  for (int i = 0; i < PickSize; i++)
  {
    int idx = dis(gen) - 1;
    int size = dis(gen);
    vecints2[idx].resize(size);
    vecpts2[idx].resize(size);
  }

  // vector element assignment
  {
    int val = 10;
    int idx1 = dis(gen) - 1;
    int idx2 = vecints2[idx1].size() / 2;
    vecints2[idx1][idx2] = val;
    if (vecints2[idx1][idx2] == val)
      std::cout << "correct assignment in vecints: " << idx1 << std::endl;
    else
      std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
  }
  {
    Point2D val(11, 15);
    int idx1 = dis(gen) - 1;
    int idx2 = vecpts2[idx1].size() / 2;
    vecpts2[idx1][idx2] = val;
    if (vecpts2[idx1][idx2] == val)
      std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
    else
      std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
  }

  stop = clock();
  ticks = stop - start; //Calculate the duration
  total = ((double)(stop - start))/CLK_TCK;
  std::cout << "std: " << total << std::endl;
  return 0;
}
