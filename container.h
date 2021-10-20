#pragma once

#include <iterator>

template<typename T>
class TArray
{
public:
    TArray(std::size_t size) : m_size(size){
        m_data = reinterpret_cast<T*>(malloc(sizeof(m_size) * m_size ));
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
    Iterator end()   { return Iterator(&m_data[m_size]); } // 200 is out of bounds

    void push_back(const T& value){
        m_size++;
        m_data = reinterpret_cast<T*>(realloc(m_data, m_size * sizeof(T)));
        if(!m_data)
            throw std::runtime_error{"Can not realloc memory"};
        m_data[m_size - 1] = value;
    }
    void clear()
    {
        free(m_data);
    }
private:
    T* m_data;
    std::size_t m_size;
};