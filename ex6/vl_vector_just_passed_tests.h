#ifndef VL_VECTOR_H
#define VL_VECTOR_H

#include <iterator>
#include <algorithm>

#define DEFAULT_STATIC_CAPACITY 16
#define OUT_OF_RANGE_MSG "Index out of range"

template<typename T, size_t StaticCapacity = DEFAULT_STATIC_CAPACITY>
class vl_vector
{
public:
    class Iterator
    {
    public:
        typedef T value_type;
        typedef T &reference;
        typedef T *pointer;
        typedef size_t difference_type;
        typedef std::forward_iterator_tag iterator_category;
        Iterator()=default;
        Iterator(T* ptr) : m_ptr(ptr) {}
        //Iterator( T* ptr) : m_ptr(ptr) {}
        T& operator*() const { return *m_ptr; }
        T* operator->() { return m_ptr; }
        Iterator& operator--() { m_ptr--; return *this; }
        Iterator operator--(int)
            { Iterator tmp = *this; --(*this); return tmp; }
        Iterator& operator-( int n) { m_ptr = m_ptr-n ; return *this; }
        Iterator& operator++() { m_ptr++; return *this; }
        Iterator operator++(int)
            { Iterator tmp = *this; ++(*this); return tmp; }
        bool operator<(const Iterator &right) const
            { return m_ptr < right.m_ptr;  }
        bool operator<=(const Iterator &right) const
            { return m_ptr <= right.m_ptr;  }
        bool operator>(const Iterator &right) const
            { return m_ptr > right.m_ptr; }
        bool operator>=(const Iterator &right) const
            { return m_ptr >= right.m_ptr; }
        friend bool operator== (const Iterator& a, const Iterator& b)
            { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b)
            { return a.m_ptr != b.m_ptr; };
    private:
        T* m_ptr;
    };
    class ConstIterator
    {
    public:
        typedef T value_type;
        typedef const T &reference;
        typedef const T *pointer;
        typedef size_t difference_type;
        typedef std::bidirectional_iterator_tag iterator_category;
        ConstIterator()=default;
        ConstIterator(const T *ptr) : m_ptr(ptr) {}
        const T& operator*() const { return *m_ptr; }
        const T* operator->() { return m_ptr; }
        ConstIterator& operator++() { m_ptr++; return *this; }
        ConstIterator operator++(int)
            { ConstIterator tmp = *this; ++(*this); return tmp; }
        ConstIterator& operator--() { m_ptr--; return *this; }
        ConstIterator operator--(int)
            { ConstIterator tmp = *this; --(*this); return tmp; }
        ConstIterator& operator-( int n) { m_ptr = m_ptr-n ; return *this; }
        bool operator<(const ConstIterator &right) const
            { return m_ptr < right.m_ptr; }
        bool operator<=(const ConstIterator &right) const
            { return m_ptr <= right.m_ptr; }
        bool operator>(const ConstIterator &right) const
            { return m_ptr > right.m_ptr; }
        bool operator>=(const ConstIterator &right) const
            { return m_ptr >= right.m_ptr; }
        friend bool operator== (const ConstIterator& a, const ConstIterator& b)
            { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const ConstIterator& a, const ConstIterator& b)
            { return a.m_ptr != b.m_ptr; };

    private:
        const T *m_ptr;
    };
    class ReverseIterator
    {
    public:
        typedef T value_type;
        typedef T &reference;
        typedef T *pointer;
        typedef size_t difference_type;
        typedef std::forward_iterator_tag iterator_category;
        ReverseIterator()=default;
        ReverseIterator(T* ptr) : m_ptr(ptr) {}
        T& operator*() const { return *m_ptr; }
        T* operator->() { return m_ptr; }
        ReverseIterator& operator++() { m_ptr--; return *this; }
        ReverseIterator operator++(int)
            { ReverseIterator tmp = *this; --(*this); return tmp; }
        bool operator<(const ReverseIterator &right) const
            { return m_ptr > right.m_ptr; }
        bool operator<=(const ReverseIterator &right) const
            { return m_ptr >= right.m_ptr; }
        bool operator>(const ReverseIterator &right) const
            { return m_ptr < right.m_ptr; }
        bool operator>=(const ReverseIterator &right) const
            { return m_ptr <= right.m_ptr; }
        friend bool operator== (const ReverseIterator& a,
                                const ReverseIterator& b)
            { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const ReverseIterator& a,
                                const ReverseIterator& b)
            { return a.m_ptr != b.m_ptr; };
    private:
        T* m_ptr;
    };



    class ConstReverseIterator
    {
    public:
        typedef T value_type;
        typedef T &reference;
        typedef T *pointer;
        typedef size_t difference_type;
        typedef std::forward_iterator_tag iterator_category;
        ConstReverseIterator()=default;
        ConstReverseIterator(T* ptr) : m_ptr(ptr) {}
        T& operator*() const { return *m_ptr; }
        T* operator->() { return m_ptr; }
        ConstReverseIterator& operator++() { m_ptr--; return *this; }
        ConstReverseIterator operator++(int)
            { ConstReverseIterator tmp = *this; --(*this); return tmp; }
        bool operator<(const ConstReverseIterator &right) const
            { return m_ptr > right.m_ptr; }
        bool operator<=(const ConstReverseIterator &right) const
            { return m_ptr >= right.m_ptr; }
        bool operator>(const ConstReverseIterator &right) const
            { return m_ptr < right.m_ptr; }
        bool operator>=(const ConstReverseIterator &right) const
            { return m_ptr <= right.m_ptr; }
        friend bool operator== (const ConstReverseIterator& a,
                                const ConstReverseIterator& b)
                                    { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const ConstReverseIterator& a,
                                const ConstReverseIterator& b)
                                { return a.m_ptr != b.m_ptr; };
    private:
        T* m_ptr;
    };

    typedef Iterator iterator;
    typedef ConstIterator const_iterator;
    typedef ReverseIterator reverse_iterator;
    typedef ConstReverseIterator const_reverse_iterator;
    const_iterator cbegin() const { return ConstIterator(&m_vector[0]); }
    const_iterator cend() const { return ConstIterator(&m_vector[m_size]); }
    const_iterator begin () const
   { return ConstIterator(&m_vector[0]); }
    iterator begin() { return Iterator(&m_vector[0]); }
    iterator end() { return Iterator(&m_vector[m_size]); }
    const_iterator end () const
    { return ConstIterator(&m_vector[m_size]); }
    reverse_iterator rbegin() { return ReverseIterator(m_vector + m_size - 1);}
    const_reverse_iterator crbegin () const
    { return ConstReverseIterator (m_vector + m_size - 1); }
    reverse_iterator rend() { return ReverseIterator(m_vector - 1); }
    const_reverse_iterator crend () const
    { return ConstReverseIterator(m_vector - 1); }

    vl_vector()
    {
        init();
    };
    vl_vector(const vl_vector<T, StaticCapacity> &src)
    {
        init();
        *this = src;
    }
    template <class InputIterator>
    vl_vector(InputIterator first, InputIterator last)
    {
        init;
        insert(begin(), first, last);
    }
    vl_vector(size_t count, const T& value)
    {
        init();
        if (count > m_capacity)
        {
            m_capacity = calc_capacity(count, 0);
            m_dynamic = new T[m_capacity];
            m_vector = m_dynamic;
        }
        std::fill_n(m_vector, count, value);
        m_size = count;
    }
    ~vl_vector()
    {
        if (m_dynamic != nullptr)
        {
            delete[] m_dynamic;
        }
    }
    size_t size() const { return m_size; };
    size_t capacity() const { return m_capacity; };
    bool empty() const { return (m_size == 0); }
    const T& at(size_t index) const
    {
        if (index >= m_size)
        {
            throw std::out_of_range(OUT_OF_RANGE_MSG);
        }
        return m_vector[index];
    }
    void push_back(const T &value)
    {
        if (m_size + 1 > m_capacity)
        {
            m_capacity = calc_capacity(m_size, 1);
            T *new_array = new T[m_capacity];
            std::copy(begin(), end(), new_array);
            if (m_dynamic != nullptr)
            {
                delete[] m_dynamic;
            }
            m_dynamic = new_array;
            m_vector = m_dynamic;
        }
        m_vector[m_size] = value;
        m_size++;
    }
    iterator insert(iterator position, const T& value)
    {
        iterator current((int *)&value);
        iterator next = current;
        return insert(position, current, ++next);
    }
    template <class InputIterator>
    iterator insert(iterator position, InputIterator first, InputIterator last)
    {
        size_t size_to_add = std::distance(first, last);
        size_t size_from_position = std::distance(position, end());
        size_t counter = 0;
        size_t size_to_position = std::distance(begin(), position);


        size_t new_size = m_size + size_to_add;
        T *dest = m_vector;
        T *new_array = nullptr;
        if (new_size > m_capacity)
        {
            m_capacity = calc_capacity(m_size, size_to_add);
            T *new_array = new T[m_capacity];
            dest = new_array;
        }

        /*
        std::copy_backward(position, cend(), &dest[new_size]);
        std::copy_backward(first, last, &dest[new_size - std::distance(position, cend())]);

        */
        //std::copy(position, cend(),&dest[new_size - position]);

        counter = 0;

        for (auto pointer = rbegin ();
            ((pointer != rend ()) && (counter < size_from_position));
            ++pointer)
        {
            //*(pointer - size_to_add) = *pointer;
            dest[new_size - counter] = *pointer;
            counter++;
        }

        std::copy(first,last,position);

        if (new_array != nullptr)
        {
            std::copy(begin(), position, &dest[0]);
            delete[] m_dynamic;
            m_dynamic = new_array;
            m_vector = m_dynamic;
        }


       // size_t t1 = std::distance(first, last);
      //  size_t t2 =  end()- t1;

      //  printf(" t1\n",t1);
      //  printf(" t2\n",t2);
      //  printf(" new_size\n",new_size);


        m_size = new_size;
      //  return iterator(&dest[new_size -
      //          std::distance(position, end()) - std::distance(first, last)]);
        return iterator(&dest[size_to_position]);
    }
    void pop_back()
    {
        if (m_size == 0) return;
        m_size--;
        if (m_size == StaticCapacity)
        {
            std::copy(begin(), end(), m_static);
            delete[] m_dynamic;
            m_dynamic = nullptr;
            m_vector = m_dynamic;
            m_capacity = StaticCapacity;
        }
    }
    iterator erase(iterator position)
    {
        iterator end_position = position;
        return erase(position, ++end_position);
    }
    iterator erase(iterator first, iterator last)
    {
        size_t size_to_remove = std::distance(first, last);
        size_t new_size = m_size - size_to_remove;
        T *dest = m_vector;
        if ((new_size <= StaticCapacity) && (m_vector != m_static))
        {
            dest = m_static;
            std::copy(begin(), first, &dest[0]);
        }
        size_t new_last = new_size - std::distance(last, end());
        std::copy(last, end(), &dest[new_last]);
        m_size = new_size;
        if ((new_size <= StaticCapacity) && (m_vector != m_static))
        {
            m_vector = m_static;
            m_capacity = StaticCapacity;
            if (m_dynamic != nullptr)
            {
                delete[] m_dynamic;
                m_dynamic = nullptr;
            }
        }
        return iterator(&dest[new_last]);
    }
    void clear()
    {
        if (m_dynamic != nullptr)
        {
            delete[] m_dynamic;
        }
        init();
    }
    T* data()
    {
        if (m_size == 0)
        {
            return nullptr;
        }
        return m_vector;
    }
    bool contains(const T &value)
    {
        return (std::find(begin(), end(), value) != end());
    }
    const T& operator[](size_t index) const
    {
        return m_vector[index];
    }
    vl_vector<T, StaticCapacity>& operator=(const vl_vector<T,
                                            StaticCapacity> &vector)
    {
        if (this == &vector)
        {
            return *this;
        }
        if (vector.size() <= StaticCapacity)
        {
            m_size = vector.size();
            m_capacity = StaticCapacity;
            m_vector = m_static;
            if (m_dynamic != nullptr)
            {
                delete[] m_dynamic;
                m_dynamic = nullptr;
            }
        }
        else
        {
            if ((m_dynamic != nullptr) && (vector.capacity() != m_capacity))
            {
                delete[] m_dynamic;
                m_dynamic = nullptr;
            }
            if (m_dynamic == nullptr)
            {
                m_capacity = vector.capacity();
                m_dynamic = new T[m_capacity];
            }
            m_size = vector.size();
            m_vector = m_dynamic;
        }
        std::copy(vector.begin(), vector.end(), m_vector);
        return *this;
    }
    bool operator==(const vl_vector<T, StaticCapacity> &rightVec) const
    {
        if (m_size == rightVec.size())
            return std::equal(rightVec.begin(), rightVec.end(), begin());
        return false;
    }
    bool operator!=(const vl_vector<T, StaticCapacity> &rightVec) const
    {
        return !(*this == rightVec);
    }
    
private:
    void init()
    {
        m_size = 0;
        m_capacity = StaticCapacity;
        m_dynamic = nullptr;
        m_vector = m_static;
    }
    static size_t calc_capacity(size_t size, size_t k)
    {
        return (3 * (size + k)) / 2;
    }

    T m_static[StaticCapacity];
    T* m_dynamic;
    T* m_vector;
    size_t m_capacity;
    size_t m_size;
};


#endif