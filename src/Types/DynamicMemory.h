//------------------------------------------------------------------------------
// File Name: DynamicMemory.h
// Author(s): Camila Basualdo Cibils 
// Vector that acts as raw memory, objects can be constructed and deconstructed 
// in place manually, does not delete automatically.
//------------------------------------------------------------------------------

/**


 BY CAMILA BASUALDO CIBILS
 ___   _______  _______    __   __  ___   __    _  _______
|   | |       ||       |  |  |_|  ||   | |  |  | ||       |
|   | |_     _||  _____|  |       ||   | |   |_| ||    ___|
|   |   |   |  | |_____   |       ||   | |       ||   |___
|   |   |   |  |_____  |  |       ||   | |  _    ||    ___|
|   |   |   |   _____| |  | ||_|| ||   | | | |   ||   |___
|___|   |___|  |_______|  |_|   |_||___| |_|  |__||_______|
 ______   _______  __    _  _______    _______  _______  __   __  _______  __   __
|      | |       ||  |  | ||       |  |       ||       ||  | |  ||       ||  | |  |
|  _    ||   _   ||   |_| ||_     _|  |_     _||   _   ||  | |  ||       ||  |_|  |
| | |   ||  | |  ||       |  |   |      |   |  |  | |  ||  |_|  ||       ||       |
| |_|   ||  |_|  ||  _    |  |   |      |   |  |  |_|  ||       ||      _||       |
|       ||       || | |   |  |   |      |   |  |       ||       ||     |_ |   _   |
|______| |_______||_|  |__|  |___|      |___|  |_______||_______||_______||__| |__|
 */
#pragma once
 /*
  * \tparam T Storage Type
  * \tparam PosType Data Type used for indexing, unsigned long long is default
  * \tparam NullMemory Set Memory to NULL on erase
 */
#include <inttypes.h>
template <typename T, typename PosType=uint32_t, bool NullMemory = false>
class DynamicMemory
{

	T* _data;
	size_t size;
	static constexpr int intPower(int value, unsigned power) {
		return power == 0 ? 1 : value * intPower(value, power - 1);
	}

	T* allocate(PosType size)
	{
		if (!size) return nullptr;
		return reinterpret_cast<T*>(new char[sizeof(T) * size]);
	}
	void destroyAlloc(T* buffer)
	{
		if (buffer)
			delete[] reinterpret_cast<char*>(buffer);
	}
	std::pair<PosType,PosType> ResizeExact(PosType newsize)
	{
		T* newdata = allocate(newsize);
		if (_data)
			memcpy(newdata,_data,size*sizeof(T));
		//memcpy_s(newdata, newsize * sizeof(T), data, size * sizeof(T));
		const PosType newbegin = size;
		const PosType newend = newsize;
		size = newsize;
		destroyAlloc(_data);
		_data = newdata;

		//RETURNS RANGE OF NEW INDICIES
		
		return std::make_pair(newbegin,newend);
	}
public:
	friend class const_iterator;
	class const_iterator
	{
		friend DynamicMemory;
		const DynamicMemory* owner;
		

	protected:
		PosType Index;


		const_iterator() : owner(nullptr), Index(0)
		{

		}
		const_iterator(const DynamicMemory* _owner, PosType _index) : owner(_owner), Index(_index)
		{

		}
	public:
		const_iterator(const const_iterator& o) = default;
		const_iterator& operator++()
		{
			Index++;
			return *this;
		}
		const_iterator& operator--()
		{
			Index--;
			return *this;
		}
		const_iterator& operator+=(PosType shift)
		{
			Index += shift;
			return *this;
		}
		const_iterator& operator-=(PosType shift)
		{
			return operator+=(-shift);
		}
		const_iterator operator+(PosType shift) const
		{
			return const_iterator(owner, Index + shift);
		}
		bool operator==(const const_iterator& o) const
		{
			return Index == o.Index;
		}
		bool operator<(const const_iterator& o) const
		{
			return Index < o.Index;
		}
		
		const T& operator*() const
		{
			return owner->operator[](Index);
		}
		PosType GetIndex() const { return Index; }

	};
	friend class iterator;
	class iterator : public const_iterator {
		DynamicMemory* owner;
	public:
		iterator() : owner(nullptr),const_iterator(nullptr,0)
		{

		}
		iterator(DynamicMemory* _owner, PosType _index) : owner(_owner),const_iterator(_owner,_index)
		{

		}
		T& operator*() const
		{
			return owner->operator[](const_iterator::Index);
			//return owner[const_iterator::Index];
		}
		
		iterator operator+(PosType shift) {
			return iterator(owner, const_iterator::Index + shift);
		}
		
		
		
	};

	
	DynamicMemory(PosType initialCapacity = 0) :size(initialCapacity), _data(allocate(initialCapacity))
	{
		Resize(initialCapacity);
	}
	DynamicMemory(PosType N, const T& val, bool construct)
	{
		Resize(N);
		if (construct)
		{
			for (int i = 0; i < Size(); i++)
				Construct(i);
		}
	}

	~DynamicMemory()
	{
		destroyAlloc(_data);

	}


	/// @brief 
	/// @param newsize 
	/// @return retuns the range of new indicies that were created
	std::pair<PosType,PosType> Resize(PosType newsize)
	{
		if (newsize == 0) return ResizeExact(0);
		else
			return ResizeExact(intPower(2, std::ceil(log2f(newsize))));
	}

	T& operator[](PosType index)
	{
		if (index < 0 || index >= size)
			throw std::out_of_range("Out of Range");
		return _data[index];
	}
	const T& operator[](PosType index) const
	{
		if (index < 0 || index >= size)
			throw std::out_of_range("Out of Range");
		return _data[index];
	}
	iterator begin()
	{
		return iterator(this, 0);
	}
	iterator end()
	{
		return iterator(this, size);
	}
	const_iterator cbegin() const {
		return const_iterator(this,0);
	}
	const_iterator cend() const {
		return const_iterator(this,size);
	}
	template<typename... Args>
	void Construct(iterator where, Args&&... args)
	{
		Construct(where.Index, std::forward<Args>(args)...);
	}
	template<typename... Args>
	void Construct(PosType where, Args&&... args)
	{
		new (_data + where) T(std::forward<Args>(args)...);
	}
	void Destroy(iterator where)
	{
		Destroy(where.Index);
	}
	void Destroy(PosType where)
	{
		_data[where].~T();

		if constexpr (NullMemory)
		{
			memset(_data + where, 0x00, sizeof(T));
		}
	}
	void* data() {return _data;}
	size_t Size() const { return size; }
	/// @brief 
	/// @param amount 
	/// @return 
	std::pair<PosType,PosType> Grow(size_t amount = 1)
	{
		return Resize(Size() + amount);
		
	}


};





