#pragma once
#include <pch.h>

template <typename T, auto comparator = std::less<T>{}>
class BST_Vector
{
public:
    using container = std::vector<T>;
    using iterator = container::iterator;
    using const_iterator = container::const_iterator;

private:
    container vec;

    iterator lower_bound(const T& value) {
        return std::lower_bound(vec.begin(), vec.end(), value,comparator);
    }
    iterator lower_bound_or_begin(const T& value) {
        auto it = lower_bound(value);
        return it != vec.end()? it : vec.begin();
    }
public:
    const_iterator Push(const T & value)
    {
        auto it = lower_bound_or_begin(value);
        vec.insert(it, value);
        return it;
    }

    template <typename... Args>
    const_iterator Emplace(Args &&...args)
    {
        T t(std::forward<Args>(args)...);
        auto it = lower_bound_or_begin(t);
        vec.insert(it, t);
        return vec.cbegin() + it;
    }

    void Erase(const_iterator it) {
        vec.erase(it);
    }

    bool Contains(const T & value)
    {
       return std::binary_search(vec.begin(),vec.end(),value,comparator);
    }
    
    const_iterator begin() const {return vec.cbegin();}
    const_iterator end() const {return vec.cend();}
    const_iterator cbegin() const {return vec.cbegin();}
    const_iterator cend() const {return vec.cend();}
    const T& front() const {return vec.front();}
    const T& back() const {return vec.back();}
    bool Empty() const {return vec.empty();}
};
