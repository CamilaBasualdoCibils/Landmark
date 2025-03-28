#pragma once

//------------------------------------------------------------------------------
// File Name: Flags.h
// Author(s): Camila Basualdo Cibils 
// simple enum flag managing class
//------------------------------------------------------------------------------
/*
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

template <typename T>
class Flags
{
public:
	using underlying_type = std::underlying_type<T>::type;
private:
	underlying_type underlyingMask;
public:
	Flags(underlying_type et) :underlyingMask(et) {}
	Flags(T t) : underlyingMask(0 | static_cast<underlying_type>(t)) {}
	Flags(std::initializer_list<T> t):underlyingMask(0)
	{
		for (auto& ts : t)
		{
			operator|=(ts);
		}
	}
	Flags() : underlyingMask(0) {};
	template <typename R>
	Flags operator|(const R& r)const 
	{
		Flags flag(underlyingMask);
		flag |= static_cast<underlying_type>(r);
		return flag;
	}
	template <typename R>
	Flags& operator|=(const R& r)
	{
		underlyingMask |= static_cast<underlying_type>(r);
		return *this;
	}

	void Set(T t, bool state) {
		if (state) {
			operator|=(t);
		}
		else {
			operator&=( ~(underlying_type)t);
		}
	}
	template <typename R>
	Flags operator&(const R& r) const
	{
		Flags flag(underlyingMask);
		flag &= static_cast<underlying_type>(r);
		return flag;
	}
	template <typename R>
	Flags& operator&=(const R& r)
	{
		underlyingMask &= static_cast<underlying_type>(r);
		return *this;
	}

	Flags operator!() const { return Flags(!underlyingMask); }

	constexpr bool operator==(const Flags& o) const { return underlyingMask == o.underlyingMask; }
	constexpr bool operator!=(const Flags& o) const { return !operator==(o); }


	constexpr underlying_type operator*() const { return underlyingMask; }

	constexpr operator bool() const { return bool(underlyingMask); }
	constexpr operator underlying_type() const { return underlyingMask; }
	
	template <typename B>
	constexpr operator vk::Flags<B>() const { return vk::Flags<B>(underlyingMask); }
	


};
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Flags<T>& obj) {
	const int bitCount = sizeof(typename Flags<T>::underlying_type) * 8;
	int firstBitSet = 0;
	for (int i = 0; i < bitCount; i++)
	{
		bool value = ((*obj) >> (bitCount - i - 1)) & 0x1;
		if (value) {
			firstBitSet = i;
			break;
		}

	}

	for (int i = firstBitSet; i < bitCount; i++)
	{
		bool value = ((*obj) >> (bitCount - i-1)) & 0x1;
		os << value;
		if (i != bitCount - 1)
			os << " | ";
	}
	return os;
}