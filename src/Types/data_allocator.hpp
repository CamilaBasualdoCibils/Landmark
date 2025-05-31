#pragma once
#include <unistd.h>
#include <vector>
#include "BST_Vector.hpp"
#include <assert.h>
#include <memory>
#include <optional>
#include <cstring>
class data_allocator
{
    public:
    using ConstructFunction = std::function<void(void *)>;    // dest,src,size
    using CopyFunction = std::function<void(void *, void *)>; // dest,src,count
    using DeconstructFunction = std::function<void(void *)>;
    using INT_TYPE = size_t;
    struct DataProperties
    {

        INT_TYPE underlying_size;
        ConstructFunction construct_func;
        CopyFunction copy_func;
        CopyFunction move_func;
        DeconstructFunction deconstruct_func;

        DataProperties() = default;
        template <typename T>
        static DataProperties FromType() {
           assert(false && "finish this function");

        }
    };
    private:
    DataProperties data_properties;
    INT_TYPE size_in_object_size;
    std::vector<std::byte> buffer;
    // std::unique_ptr<std::byte[]> buffer;
    //  std::vector<char> data;
    using sector = std::pair<INT_TYPE, INT_TYPE>; // begin->end free sector

    static constexpr bool sector_compare(const sector &a, const sector &b)
    {
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
        bool operator==(const iterator &other) const { return index == other.index; }
        auto operator<=>(const iterator &other) const { return index <=> other.index; }

        void *operator*()
        {
            return owner->operator[](index);
        }
        template <typename T>
        T *Get()
        {
            return reinterpret_cast<T *>(this->operator*());
        }
        INT_TYPE GetIndex() const { return index; }
    };

    data_allocator(size_t underlying_size);
    data_allocator(const DataProperties &data_properties);
    bool Contains(INT_TYPE index) const;
    template <typename T>
    iterator Push(const T &val);
    template <typename... Args>
    iterator Push_Emplace(Args &&...args);
    //template <typename T>
    //iterator Insert(INT_TYPE index, const T &);
    iterator Insert(INT_TYPE index);
        template <typename T,typename... Args>
    iterator Insert_Emplace(INT_TYPE index,Args &&...args);
    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, size_in_object_size); }
    void *operator[](INT_TYPE index);
    size_t GetNextInsertLocation() const;
    std::optional<sector> Get_Empty_sector_here(INT_TYPE a);
    iterator ReserveIndex(size_t index);

private:
    static bool CompareSectorIndex(const sector &s, INT_TYPE a);
    auto GetContainingSector(INT_TYPE index) const;
    void IncreaseCapacity();


    size_t Size() const;
};

template <typename T>
inline data_allocator::iterator data_allocator::Push(const T &val)
{
    assert(sizeof(T) == data_properties.underlying_size  && "Invalid Type");
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

template <typename T,typename... Args>
inline data_allocator::iterator data_allocator::Insert_Emplace(INT_TYPE index,Args &&...args)
{
    const size_t offset = index * data_properties.underlying_size;
    if (Contains(index) && data_properties.deconstruct_func) {
        data_properties.deconstruct_func(buffer.data() +offset);
    }
    else ReserveIndex(index);
    new (buffer.data() + offset) T(std::forward<Args>(args)...);
    return iterator(this,index);
}
