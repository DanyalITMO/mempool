#pragma once

#include <iterator>
#include <cstring>

template<typename T, typename allocator = std::allocator<T>>
class TArray
{
public:
    TArray(std::size_t size) : m_size(size){
        if(m_size)
        {
            m_data = m_allocator.allocate(m_size);
            for(std::size_t i = 0; i < m_size; i++)
            {
//                std::cout<<std::hex<<"construct: "<<m_data + i<<std::endl;
                m_allocator.construct(m_data + i, 1);
            }
        }
    }

    TArray() : TArray(200)
    {}

    ~TArray(){
        clear();
    }

    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;  // or also value_type*
        using reference         = T&;  // or also value_type&

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        // Prefix increment
        Iterator& operator++() { m_ptr++; return *this; }

        // Postfix increment
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

    private:

        pointer m_ptr;
    };

    Iterator begin() { return Iterator(&m_data[0]); }
    Iterator end()   { return Iterator(&m_data[m_size]); }

    void push_back(const T& value){

        auto new_data = m_allocator.allocate(m_size + 1);
        if(!new_data)
            throw std::runtime_error{"Can not realloc memory"};
        for(std::size_t i = 0; i < m_size; i++) {
            m_allocator.construct(new_data + i, *reinterpret_cast<T *>(m_data + i));
            m_allocator.destroy(reinterpret_cast<T *>(m_data + i));
        }
        m_allocator.construct(new_data + m_size, value);

        clear();
        m_data = new_data;
        m_size++;

    }
    void clear()
    {
        if(m_size)
        {
            m_allocator.deallocate(m_data, m_size);
        }
    }
private:
    T* m_data = nullptr;
    std::size_t m_size;
    allocator m_allocator;
};