#ifndef _MY_ALLOCATOR
#define _MY_ALLOCATOR

#include"mempool.hpp"
#include <bits/stl_algobase.h>
#include <bits/allocator.h>
#include <bits/stl_construct.h>
#include <bits/stl_uninitialized.h>

template <class T>
class myAllocator
{
public:
    typedef void _Not_user_specialized;
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::true_type is_always_equal;
    template<typename _Tp1>
	  struct rebind
	  { typedef myAllocator<_Tp1> other; };
public:
    static MemPool * mempool;

public:
    myAllocator() noexcept{}
    myAllocator( const myAllocator& other ) noexcept{}
    template< class U >
    myAllocator( const myAllocator<U>& other ) noexcept{}
    ~myAllocator(){}

    pointer address(reference _Val) const noexcept
    {return std::__addressof(_Val);}
    const_pointer address(const_reference _Val) const noexcept
    {return std::__addressof(_Val);}
    void deallocate(pointer _Ptr, size_type _Count)
    {
      // ::operator delete(_Ptr);
      this->mempool->Deallocate(_Ptr,_Count*sizeof(T));
    }

    pointer allocate(size_type _Count)
    {
	    // return static_cast<T*>(::operator new(_Count * sizeof(T)));
      return static_cast<T*>(this->mempool->Allocate(_Count*sizeof(T)));
    }

    template<class U> void destroy(U *_Ptr){_Ptr->~U();}
    template< class U, class... Args >
    void construct( U* p,Args&&... args )
    {
    ::new((void *)p) U(std::forward<Args>(args)...);
    }
};

 template<typename _T1, typename _T2>
    inline bool
    operator==(const myAllocator<_T1>&, const myAllocator<_T2>&)
    noexcept
    { return true; }

  template<typename _Tp>
    inline bool
    operator==(const myAllocator<_Tp>&, const myAllocator<_Tp>&)
    noexcept
    { return true; }

  template<typename _T1, typename _T2>
    inline bool
    operator!=(const myAllocator<_T1>&, const myAllocator<_T2>&)
    noexcept
    { return false; }

  template<typename _Tp>
    inline bool
    operator!=(const myAllocator<_Tp>&, const myAllocator<_Tp>&)
    noexcept
    { return false; }

    
// Must be defined BEFORE including <vector>
namespace std{
  template<typename _ForwardIterator, typename _Size, typename _Tp>
    inline _ForwardIterator
    __uninitialized_default_n_a(_ForwardIterator __first, _Size __n, 
				myAllocator<_Tp>&)
    { return std::__uninitialized_default_n(__first, __n); }

  template<typename _ForwardIterator, typename _Tp>
    inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last,
	     myAllocator<_Tp>&)
    {
      _Destroy(__first, __last);
    }
}

#endif