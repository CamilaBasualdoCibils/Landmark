
#include <pch.h>

template <typename T>

class bounded_vector
{
    size_t Min = 0,Max = 0;
    std::vector<T> vector;
    //std::unordered_set<size_t> EmptySlots;

    public:
    

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    size_t size() const {return Max-Min;}
    size_t capacity() const {return vector.size();}

    void resize(size_t newMin,size_t newMax);
    void resize_max(size_t newMax);
    void resize_min(size_t newMin);
};


