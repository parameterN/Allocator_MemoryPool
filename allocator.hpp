#ifndef _MY_ALLOCATOR
#define _MY_ALLOCATOR
#include<iostream>


template <class T>
class MyAllocator
{
private:
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

    MyAllocator() noexcept;
    MyAllocator( const allocator& other ) noexcept;
    template< class U >
    allocator( const allocator<U>& other ) noexcept;
    ~allocator();

    pointer address(reference _Val) const noexcept;
    const_pointer address(const_reference _Val) const noexcept;
    void deallocate(pointer _Ptr, size_type _Count);
    pointer allocate(size_type _Count);
    template<class _Uty> void destroy(_Uty *_Ptr);
    template<class _Objty, class _Types>;
    void construct(_Objty *_Ptr, _Types&&... _Args);

public:

};


#endif