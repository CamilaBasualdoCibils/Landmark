#include "data_allocator.hpp"
#include <exception>
#include <memory>
#include <cstring>
data_allocator::iterator data_allocator::Insert_Null(INT_TYPE index)
{
    assert(!Contains(index) && "Index already taken");
    ReserveIndex(index);
    const size_t offset = index * underlying_size;
    std::memset(buffer.data() +offset ,69,underlying_size);
    return iterator(this,index);
}
void *data_allocator::operator[](INT_TYPE index)
{
    assert(Contains(index)&&"INVALID INDEX");
    return buffer.data() + index*underlying_size;
}
bool data_allocator::CompareSectorIndex(const sector &s, INT_TYPE a)
{
    //idk chatgpt said this
    return s.second <= a;
}
auto data_allocator::GetContainingSector(INT_TYPE index) const
{
    auto it = std::lower_bound(free_sectors.cbegin(), free_sectors.cend(), index, CompareSectorIndex);
    if (it == free_sectors.cend()) return it;
    if ((*it).first <= index && (*it).second > index) //sector contains index
        return it;
    if (it != free_sectors.begin() && (*it).first <= index && (*it).second > index) //try the previous
        return --it;
    return free_sectors.cend();//none found
}
void data_allocator::IncreaseCapacity()
{


    const size_t previous_size = size_in_object_size;
    const size_t new_size = previous_size != 0 ? previous_size*2 : 1;
    buffer.resize(new_size*underlying_size,std::byte(0));
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

data_allocator::data_allocator(size_t data_size):underlying_size(data_size)
{
    size_in_object_size = 0;
    IncreaseCapacity();
    
}


bool data_allocator::Contains(INT_TYPE index) const
{
    auto containing_sector = GetContainingSector(index);
    bool sector_found = containing_sector  != free_sectors.cend(); //if sector was not found means index is taken
    return Size() > index && !sector_found;
}

size_t data_allocator::GetNextInsertLocation() const
{
    for (const auto &sector : free_sectors)
    {
        return sector.first;
    }
    return Size(); // no free location available. first index of next increase will be available
}

void data_allocator::ReserveIndex(size_t index)
{
    while (Size() <= index)
    {
        IncreaseCapacity();
    }

    auto it = GetContainingSector(index);
    if (it == free_sectors.cend())
        throw "INVALID INDEX";
    sector s = *it;

    free_sectors.Erase(it);

    if (s.first + 1 == s.second)
        return; // sector of size one. just remove

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
