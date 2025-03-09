#pragma once
#include <vector>
#include <set>
#include <stdexcept>
#include <cstring>
#include <minmax.h>


template<typename T>
class lobby_Decrapated
{
public:
	typedef size_t pos_type;
	typedef std::set<pos_type>::iterator marker_iterator;
	typedef typename std::vector<T>::iterator data_iterator;
private:

	std::vector<T> _vector;
	std::set<pos_type> _markers;



public:
	friend class iterator;
	class iterator //: public std::iterator<std::random_access_iterator_tag, pos_type, T, T*, T&>
	{
		friend lobby_Decrapated;
	protected:
		std::vector<T>::const_iterator it;
		pos_type index;
		const lobby_Decrapated* owner;

		//iterator();
		iterator(lobby_Decrapated* _owner, pos_type _index) :owner(_owner), index(_index) {
			it = owner->_vector.begin() += index;

		}

	public:

		/*
		OPERATORS ++ AND -- DO NOT WORK. YOU HAVE TO STORE 2 SET ITERATORS. ONE AHEAD AND ONE BEHIND*/
		iterator& operator++() {
			auto uppermarker = owner->_markers.upper_bound(index);
			auto end = owner->_markers.end();



			do {
				if (!owner->_markers.empty() && uppermarker != end)
					uppermarker++;
				index++;
				it++;
			} while (((!owner->_markers.empty() &&uppermarker != end) && *uppermarker == index));

			return *this;
		}


		iterator& operator--() {
			auto lowermarker = owner->_markers.lower_bound(index);

			while (*lowermarker == index) {

				lowermarker--;
				index--;
			}

			return *this;
		}


		T& operator*() {
			auto t = *it;
			return t;
		}

		bool operator!=(const iterator& other) {
			bool state = it != other.it;
			return state;
		}

	};
	friend class address;
	class address
	{
		friend lobby_Decrapated;
	public:
		address() :_owner(nullptr), Location(-1) {

		}
		template <typename O>
		address(O& other) 
			:_owner(reinterpret_cast<lobby_Decrapated<T>* const>(static_cast<void* const>(other.GetOwner())))
		,Location(other.GetLocation()){
		}
	protected:
		address(lobby_Decrapated* owner, pos_type pos) : _owner(owner), Location(pos)
		{

		}
		lobby_Decrapated* const _owner;
		const pos_type Location;

	public:
		
		// template <typename A> operator lobby<A>::address() {
		//	 return lobby<A>::address(static_cast<A>(*this));
		//}
		lobby_Decrapated* const GetOwner() const {   return _owner; }
		pos_type GetLocation() const { return Location; }
		T& operator*() { return _owner->operator[](Location); }
		bool Valid() { return _owner && (Location >= 0 && Location < _owner._vector.size()) &&!_markers.contains(Location); }
	};

	lobby_Decrapated(size_t capacity = 0) : _vector(capacity), _markers()
	{

	}

	//WRONG
	iterator begin() {

		if (_vector.empty() || _markers.empty())
			return iterator(this, (pos_type)0);

		pos_type pos = 0;
		auto it = _vector.begin();
		while (_markers.contains(pos))
		{
			pos++;
			it++;
			if (pos >= _vector.size())
				return iterator(this, (pos_type)0);
		}

		return iterator(this, pos);
	}
	//WRONG

	iterator End() {
		if (_vector.empty() || _markers.empty())
			return iterator(this, max(0, (pos_type)_vector.size() - 1));

		pos_type pos = _vector.size() - 1;
		auto it = _vector.end();
		while (_markers.contains(pos))
		{
			pos--;
			it--;
			if (pos == 0)
				return iterator(this, (pos_type)0);

		}

		return iterator(this, pos);

	}

	T* data() { return _vector.data(); }

	/// @brief 
	/// @return Size of underlying vector
	size_t Size() {
		return _vector.size();
	}
	/// @brief 
	/// @return number of valid entries within the underlying vector 
	size_t Count() {
		return Size() - _markers.size();
	}


	/// @brief inserts a element into the container
	/// @param val value to insert
	/// @return index at which it was added and is guaranteed to never change for as long as element exists in container
	address Push(const T& val) {
		if (_markers.size() > 0)
		{
			pos_type insertPos = *_markers.begin();
			memcpy(&_vector[insertPos], &val, sizeof(T));
			//_vector[insertPos]=  val;
			_markers.erase(_markers.begin());
			return address(this, insertPos);
			//return iterator(_vector.begin() + insertPos, (pos_type)insertPos, _markers);
		}
		else {
			_vector.push_back(val);
			return address(this, _vector.size() - 1);
		}
	}


	/// @brief 
	/// @param pos position in lobby to check 
	/// @return whether position contains an active entry
	bool Exists(const pos_type pos) {
		if (pos >= 0 && pos < _vector.size() && _markers.find(pos) != _markers.end()) return true;
		return false;
	}
	/// @brief 
	/// @param pos position in lobby to check 
	/// @return whether position contains an active entry
	bool Exists(const iterator pos) {
		return Exists(*pos);
	}



	/// @brief 
	/// @param pos marks a location as empty and ready for overwriting. calls destructor of template type
	void Erase(const pos_type pos) {
		if (std::is_destructible<T>::value)
			_vector[pos].~T();
		_markers.emplace(pos);
	}



	T& operator[](pos_type index) {
		if (_vector.size() <= index) throw std::out_of_range("Index is outside the bounds of the array");

		if (_markers.find(index) != _markers.end())throw std::out_of_range("Index is marked as empty. retrieving not allowed!");

		return _vector[index];
	}

};
