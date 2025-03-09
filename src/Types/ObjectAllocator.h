#ifndef OBJECTALLOCATOR_H
#define OBJECTALLOCATOR_H
#include <unordered_map>
template <typename T, typename KeyType = uint32_t, bool NullMemory = false>
class ObjectAllocator
{
	
	std::unordered_map<KeyType, T> data;
	std::queue<KeyType> releasedValues;
	KeyType GetLowestKeyNotUsed() const
	{
		if (releasedValues.empty())
			return data.size();
		else return releasedValues.front();
	}
	KeyType ReserveNextKey()
	{
		if (releasedValues.empty())
			return data.size();
		else {
			const auto ret = releasedValues.front();
			releasedValues.pop();
			return ret;
		}
	}
public:

	using iterator = typename decltype(data)::iterator;
	using const_iterator = typename decltype(data)::const_iterator;

	bool Contains(KeyType index) const
	{
		return data.contains(index);
	}
	iterator Push(const T& val)
	{
		const KeyType key = ReserveNextKey();

		
		return Insert(val);

	}

	template<typename... Args>
	iterator Push_Emplace(Args&&... args)
	{
		const KeyType key = ReserveNextKey();

		return Insert_Emplace(key, std::forward<Args>(args)...);

	}
	iterator Insert(KeyType where, const T& v)
	{
		data[where] = v;
		return data.find(where);
	}
	iterator Insert(const iterator& where, const T& val)
	{
		return Insert(where.Index());
	}
	template<typename... Args>
	iterator Insert_Emplace(KeyType where, Args&&... args)
	{
		//data.emplace()
		data.emplace(std::piecewise_construct,
                     std::forward_as_tuple(where),
                     std::forward_as_tuple(std::forward<Args>(args)...));
		return data.find(where);
	}
	template<typename... Args>
	iterator Insert_Emplace(const iterator& where, Args&&... args)
	{
		return Insert(where.Index());
	}

	void Erase(iterator where)
	{
		Erase(where.Index());
	}

	void Erase(KeyType where)
	{
		releasedValues.push(where);
		data.erase(where);
	}

	iterator begin() { return data.begin(); }
	const_iterator begin() const { return data.begin(); }

	iterator end() { return data.end(); }
	const_iterator end() const { return data.end(); }

	T& operator[](KeyType where) { return data.at(where); }
	const T& operator[](const KeyType& where) const { return data.at(where); }

	iterator find(KeyType what) { return data.find(what); }
	const_iterator find(KeyType what) const { return data.find(what); }
};
#endif // OBJECTALLOCATOR_H
