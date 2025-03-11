#pragma once
#include <unistd.h>
#include <vector>
#include "BST_Vector.hpp"
#include <assert.h>
#include <memory>
#include <optional>
class data_allocator
{


    using INT_TYPE = size_t;
    const INT_TYPE underlying_size;
    INT_TYPE size_in_object_size;
    std::vector<std::byte>buffer;
    //std::unique_ptr<std::byte[]> buffer;
    // std::vector<char> data;
    using sector = std::pair<INT_TYPE, INT_TYPE>; // begin->end free sector

    static constexpr bool sector_compare(const sector& a, const sector&b) {
        return a.first < b.first;
    }

    BST_Vector<sector, sector_compare>
        free_sectors;

public:
    struct iterator
    {
        data_allocator *owner;
        INT_TYPE index;
        iterator(data_allocator *owner, INT_TYPE index);
        iterator &operator++(void)
        {
            index++;
            return *this;
        }
        iterator operator++(int)
        {
            iterator temp = *this;
            index++;
            return temp;
        }
        iterator operator+(INT_TYPE i)
        {
            iterator b = *this;
            b++;
            return b;
        }
        bool operator==(const iterator& other) const {return index == other.index;}
        auto operator<=>(const iterator &other) const { return index <=> other.index; }

        void* operator*() {
            return owner->operator[](index);
        }
        template <typename T>
        T* Get() {
            return reinterpret_cast<T*>(this->operator*());
        }
        INT_TYPE GetIndex() const {return index;}
    };

    data_allocator(size_t data_size);
    bool Contains(INT_TYPE index) const;
    template <typename T>
    iterator Push(const T &val);
    template <typename... Args>
    iterator Push_Emplace(Args &&...args);
    template <typename T>
    iterator Insert(INT_TYPE index, const T &);
    iterator Insert_Null(INT_TYPE index);
    iterator begin() {return iterator(this,0);}
    iterator end() {return iterator(this,size_in_object_size);}
    void* operator[](INT_TYPE index);
    size_t GetNextInsertLocation() const;
    std::optional<sector> Get_Empty_sector_here(INT_TYPE a);
private:
    static bool CompareSectorIndex(const sector &s, INT_TYPE a);
    auto GetContainingSector(INT_TYPE index) const;
    void IncreaseCapacity();
    
    void ReserveIndex(size_t index);

    size_t Size() const;
};

template <typename T>
inline data_allocator::iterator data_allocator::Push(const T &val)
{
    assert(sizeof(T) <= underlying_size && "Invalid Type");
    const INT_TYPE key = GetNextInsertLocation();
    ReserveIndex(key);
    return Insert(key, val);
}
template <typename... Args>
inline data_allocator::iterator data_allocator::Push_Emplace(Args &&...args)
{
    const INT_TYPE key = GetNextInsertLocation();
    ReserveIndex(key);

    return Insert_Emplace(key, std::forward<Args>(args)...);
}

template <typename T>
inline data_allocator::iterator data_allocator::Insert(INT_TYPE index, const T &v)
{
    auto it = Insert_Null(index);
    new (buffer.data() + index * underlying_size) T(v);
    return it;
    
}
