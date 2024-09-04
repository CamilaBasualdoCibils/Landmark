#ifndef OBJECTALLOCATOR_NEW_H
#define OBJECTALLOCATOR_NEW_H
// #include <unordered_map>
#include <inttypes.h>
#include "DynamicMemory.h"
// #include <queue>
#include <set>
template <typename T, typename KeyType = uint32_t, bool NullMemory = false>
class ObjectAllocator_new
{
	DynamicMemory<T, KeyType, NullMemory> _data;
	// std::unordered_map<KeyType, T> data;
	std::set<KeyType> releasedValues;
	// std::queue<KeyType> releasedValues;
	KeyType GetLowestKeyNotUsed() const
	{
		if (releasedValues.empty())
			return _data.Size();
		else
			return *releasedValues.begin();
	}
	KeyType ReserveNextKey()
	{
		if (_data.Size() == 0 || releasedValues.empty())
		{
			auto newindicies = _data.Grow();
			RegisterNewEntries(newindicies);
		}

		auto begin = releasedValues.begin();
		const auto ret = *begin;
		releasedValues.erase(begin);
		return ret;
	}
	void EnsureSpaceForKey(KeyType key)
	{
		if (_data.Size() <= key)
		{
			RegisterNewEntries(_data.Resize(key + 1));
		}
	}

	void RegisterNewEntries(std::pair<KeyType, KeyType> range)
	{
		for (int i = range.first; i != range.second; i++)
			releasedValues.insert(i);
	}

public:
	using iterator = typename decltype(_data)::iterator;
	using const_iterator = typename decltype(_data)::const_iterator;

	ObjectAllocator_new() = default;
	ObjectAllocator_new(const std::initializer_list<std::pair<KeyType, T>> &initializer)
	{
	}

	bool Contains(KeyType index) const
	{
		if (index >= _data.Size())
			return false;
		return !releasedValues.contains(index);
	}
	iterator Push(const T &val)
	{
		const KeyType key = ReserveNextKey();

		return Insert(key, val);
	}

	template <typename... Args>
	iterator Push_Emplace(Args &&...args)
	{
		const KeyType key = ReserveNextKey();

		return Insert_Emplace(key, std::forward<Args>(args)...);
	}
	iterator Insert(KeyType where, const T &v)
	{
		EnsureSpaceForKey(where);
		_data[where] = v;
		return _data.begin() + where;
	}
	iterator Insert(const iterator &where, const T &val)
	{
		return Insert(where.Index());
	}
	template <typename... Args>
	iterator Insert_Emplace(KeyType where, Args &&...args)
	{
		// data.emplace()
		EnsureSpaceForKey(where);
		_data.Construct(where, std::forward<Args>(args)...);
		// data.emplace(std::piecewise_construct,
		//              std::forward_as_tuple(where),
		//              std::forward_as_tuple(std::forward<Args>(args)...));
		return _data.begin() + where;
	}

	void *data() { return _data.data(); }
	void Erase(iterator where)
	{
		Erase(where.Index());
	}

	void Erase(KeyType where)
	{
		//releasedValues.push(where);
		// data.erase(where);
		releasedValues.emplace(where);
		_data.Destroy(where);
	}

	iterator begin() { return _data.begin(); }
	const_iterator begin() const { return _data.cbegin(); }

	iterator end() { return _data.end(); }
	const_iterator end() const { return _data.cend(); }

	T &operator[](KeyType where) { return _data[where]; }
	const T &operator[](const KeyType &where) const { return _data[where]; }

	iterator find(KeyType what) { return _data.begin() + what; }
	const_iterator find(KeyType what) const { return _data.cbegin() + what; }

	size_t Count() const { return _data.Size() - releasedValues.size() ;}
	size_t Size() const { return _data.Size(); }
};
#endif // OBJECTALLOCATOR_H
