#ifndef _MY_ALLOCATOR
#define _MY_ALLOCATOR
#include<iostream>
#include"mempool.hpp"

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
    // typedef true_type propagate_on_container_move_assignment;
    // typedef true_type is_always_equal;

public:
    static MemPool * mempool;

public:
    myAllocator() noexcept{}
    myAllocator( const myAllocator& other ) noexcept{}
    // template< class U >
    // allocator( const myAllocator<U>& other ) noexcept;
    ~myAllocator(){}

    // pointer address(reference _Val) const noexcept;
    // const_pointer address(const_reference _Val) const noexcept;
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

#endif