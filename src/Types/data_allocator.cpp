#include "data_allocator.hpp"
#include <exception>
#include <memory>
#include <cstring>
data_allocator::iterator data_allocator::Insert(INT_TYPE index)
{
    assert(!Contains(index) && "Index already taken");
    ReserveIndex(index);
    const size_t offset = index * data_properties.underlying_size;
    if (data_properties.construct_func)
        data_properties.construct_func(buffer.data() + offset);
    return iterator(this, index);
}
void *data_allocator::operator[](INT_TYPE index)
{
    const bool bContains = Contains(index);
    assert(bContains && "INVALID INDEX");
    return buffer.data() + index * data_properties.underlying_size;
}
bool data_allocator::CompareSectorIndex(const sector &s, INT_TYPE a)
{
    // idk chatgpt said this
    return s.second <= a;
}
auto data_allocator::GetContainingSector(INT_TYPE index) const
{
    auto it = std::lower_bound(free_sectors.cbegin(), free_sectors.cend(), index, CompareSectorIndex);
    if (it == free_sectors.cend())
        return it;
    if ((*it).first <= index && (*it).second > index) // sector contains index
        return it;
    if (it != free_sectors.begin() && (*it).first <= index && (*it).second > index) // try the previous
        return --it;
    return free_sectors.cend(); // none found
}
void data_allocator::IncreaseCapacity()
{

    const size_t previous_size = size_in_object_size;
    const size_t new_size = previous_size != 0 ? previous_size * 2 : 1;
    std::vector<std::byte> new_buffer(new_size * data_properties.underlying_size, std::byte(0));
    for (int i = 0; i < previous_size; i++)
    {
        size_t offset = i * data_properties.underlying_size;
        data_properties.move_func(new_buffer.data() + offset, buffer.data() + offset);
        if (data_properties.deconstruct_func)
            data_properties.deconstruct_func(buffer.data() + offset);
    }
    buffer = std::move(new_buffer);
    size_in_object_size = new_size;
    if (previous_size != 0 && !free_sectors.Empty() && free_sectors.back().second == previous_size)
    {
        sector s = free_sectors.back();
        free_sectors.Erase(free_sectors.end() - 1);
        s.second = new_size;
        free_sectors.Push(s);
    }
    else
    {
        sector new_sector = {previous_size, new_size};
        free_sectors.Push(new_sector);
    }
}

data_allocator::data_allocator(size_t underlying_size)
{
    data_properties.underlying_size = underlying_size;
    data_properties.construct_func = nullptr;
    data_properties.deconstruct_func = nullptr;
    data_properties.copy_func = [size = data_properties.underlying_size](void *dest, void *src)
    { std::memcpy(dest, src, size); };
    data_properties.move_func = [size = data_properties.underlying_size](void *dest, void *src)
    { std::memcpy(dest, src, size); };
    size_in_object_size = 0;
    IncreaseCapacity();
}

data_allocator::data_allocator(const DataProperties &properties)
{
    assert(properties.construct_func);
    assert(properties.copy_func);
    assert(properties.move_func);
    assert(properties.deconstruct_func);
    assert(properties.underlying_size >= 1);

    data_properties = properties;
    size_in_object_size = 0;
    IncreaseCapacity();
}

bool data_allocator::Contains(INT_TYPE index) const
{

    return Size() > index && (free_sectors.Empty() || (GetContainingSector(index) == free_sectors.cend()));
}

size_t data_allocator::GetNextInsertLocation() const
{
    for (const auto &sector : free_sectors)
    {
        return sector.first;
    }
    return Size(); // no free location available. first index of next increase will be available
}

std::optional<data_allocator::sector> data_allocator::Get_Empty_sector_here(INT_TYPE a)
{
    std::optional<data_allocator::sector> op;
    auto it = GetContainingSector(a);
    if (it != free_sectors.cend())
        return op = *it;
    return op;
}

data_allocator::iterator data_allocator::ReserveIndex(size_t index)
{
    while (Size() <= index)
    {
        IncreaseCapacity();
    }
    const iterator ret = iterator(this,index);
    auto it = GetContainingSector(index);
    if (it == free_sectors.cend())
        throw "INVALID INDEX";
    sector s = *it;

    free_sectors.Erase(it);

    if (s.first + 1 == s.second)
        return ret; // sector of size one. just remove

    if (s.first == index)
    {
        s.first++;
    }
    else if (s.second == index + 1)
    {
        s.second--;
    }
    else
    {
        INT_TYPE previous_end = s.second;

        s.second = index;
        sector new_sector = {index + 1, previous_end};
        free_sectors.Push(new_sector);
    }
    free_sectors.Push(s);
    return ret;
}

size_t data_allocator::Size() const
{
    return size_in_object_size;
}

data_allocator::iterator::iterator(data_allocator *_owner, INT_TYPE _index)
{
    owner = _owner;
    index = _index;
}
