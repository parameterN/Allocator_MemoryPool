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

public:
    MyAllocator() noexcept;
    MyAllocator( const MyAllocator& other ) noexcept;
    // template< class U >
    // allocator( const MyAllocator<U>& other ) noexcept;
    ~MyAllocator();

    // pointer address(reference _Val) const noexcept;
    // const_pointer address(const_reference _Val) const noexcept;
    void deallocate(pointer _Ptr, size_type _Count);
    pointer allocate(size_type _Count);
    template<class U> void destroy(U *_Ptr);
    template< class U, class... Args >
    void construct( U* p, Args&&... args );

};

template <class T>
MyAllocator<T>::MyAllocator() noexcept{}
template <class T>
MyAllocator<T>::MyAllocator( const MyAllocator& other ) noexcept{}
template <class T>
MyAllocator<T>::~MyAllocator(){}
template <class T>
void MyAllocator<T>::deallocate(pointer _Ptr, size_type _Count)
{
    ::delete _Ptr;
}
template <class T>
typename MyAllocator<T>::pointer MyAllocator<T>::allocate(size_type _Count)
{
	return static_cast<T*>(::operator new(_Count * sizeof(T)));
}
template<class T>
template<class U>
void MyAllocator<T>::destroy(U *_Ptr){_Ptr->~U();}

template<class T>
template< class U, class... Args >
void MyAllocator<T>::construct( U* p, Args&&... args )
{
   ::new((void *)p) U(std::forward<Args>(args)...);
}

#endif