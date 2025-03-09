#pragma once
#include <deque>
#include <vector>
#include <mutex>
#include <numeric>
#include <array>
#include <functional>

template <typename T, int Size>
class ReadWriteStack
{
	using ObjectID = uint32_t;
	//objects In Raw Format
	std::array<uint8_t,Size*sizeof(T)> objects;
	//views for debugging
	std::array<T,Size>& objectViews = reinterpret_cast<std::array<T, Size>&>(objects);
	//vectors in order of first to use for writing, last for reading
	//writing begin takes from front, end pushes to back
	//reading begin takes from back, end pushes to front
	std::deque<ObjectID> ObjectQueue = initialize_Queue();
	std::vector<ObjectID> ObjectsInUse;

	std::mutex ReserveFetchMutex;
public:
	friend class Cursor;
	class Cursor
	{
		friend ReadWriteStack;
		ReadWriteStack* owner;
		ObjectID objectHandle;
		bool Valid;
	protected:
		enum Usage {READ,WRITE};
		const Usage usage;
		Cursor(ReadWriteStack* _owner, ObjectID _objectHandle,Usage _usage) :owner(_owner),objectHandle(_objectHandle), Valid(true),usage(_usage) {}
		//Cursor(const Cursor& o):;
		Cursor& operator=(const Cursor& o)
		{
			if (usage != o.usage)
			{
				throw std::exception("This should not be possible");
			}
			owner = o.owner;
			objectHandle = o.objectHandle;
			Valid = o.Valid;
			return *this;

		};
		constexpr void Invalidate() { Valid = false; }
		//constexpr Usage GetUsage() const { return usage; }

		constexpr ReadWriteStack& GetOwner() { return *owner; }
		constexpr const ReadWriteStack& GetOwner() const { return *owner; }

		T& Get()
		{
			if (!Valid) throw std::runtime_error("Cursor Is invalid");
			return owner->GetObject(objectHandle);
		}
		const T& Get() const
		{
			if (!Valid) throw std::runtime_error("Cursor Is invalid");
			return owner->GetObject(objectHandle);
		}

	public:

		constexpr ObjectID GetID() const { return objectHandle; }
		constexpr  bool isValid() const { return Valid; }
	};

	class ReadCursor : public Cursor
	{
		friend ReadWriteStack;
	protected:
		ReadCursor(ReadWriteStack* _owner, ObjectID handle) : Cursor(_owner,handle, Cursor::Usage::READ) {}

	public:
		
		const T& operator*() const { return Cursor::Get(); }
		const T* operator->() const { return &Cursor::Get(); }
	};

	class WriteCursor : public Cursor
	{
		friend ReadWriteStack;
		WriteCursor(ReadWriteStack* _owner,ObjectID handle) : Cursor(_owner,handle, Cursor::Usage::WRITE) {}
	public:
		WriteCursor(const WriteCursor& o) = default;
		WriteCursor& operator=(const ReadCursor& o) =delete;
		//WriteCursor& operator=(const WriteCursor& o) { return Cursor::operator=(o); };
		T& operator*() { return Cursor::Get(); }
		 T* operator->()  { return &Cursor::Get(); }
	};

	ReadWriteStack() {}

	template <typename... Args>
	ReadWriteStack(Args&&... args) :objects(0)
	{
		for (int i = 0; i < Size;i++)
		{
			auto objectPtr = &GetObject(i);
			new (objectPtr) T(std::forward<Args>(args)...);
		}

	}
	~ReadWriteStack()
	{
		for (int i = 0; i < Size; i++)
		{
			GetObject(i).~T();
		}
	}
	WriteCursor RequestWrite()
	{
		std::unique_lock lock(ReserveFetchMutex);

		const ObjectID ID = ObjectQueue.front();
		ObjectQueue.pop_front();
		ObjectsInUse.push_back(ID);
		return WriteCursor(this,ID);

	}

	ReadCursor RequestRead()
	{
		std::unique_lock lock(ReserveFetchMutex);

		const ObjectID ID = ObjectQueue.back();
		ObjectQueue.pop_back();
		ObjectsInUse.push_back(ID);
		return ReadCursor(this,ID);
	}



	void Done(Cursor& cursor)
	{
		std::unique_lock lock(ReserveFetchMutex);

		auto find = std::find(ObjectsInUse.begin(), ObjectsInUse.end(), cursor.GetID());
		if (find == ObjectsInUse.end())
			throw std::logic_error("Internal Failure");

		ObjectsInUse.erase(find);


		std::deque<ObjectID>::iterator PushTo = cursor.usage == Cursor::Usage::WRITE ? ObjectQueue.end() : ObjectQueue.begin();

		ObjectQueue.insert(PushTo, cursor.GetID());
	}
	/*
	template< typename ReturnType, typename... Args>
	std::array< ReturnType,Size> Call(ReturnType(T::* memberFunction)(Args...), Args... args) {
		// Use std::invoke to call the member function with provided arguments
		std::array< ReturnType, Size> returnvalues;
		for (int i = 0 ; i < Size;i++)
		{
			returnvalues[i] = std::invoke(memberFunction, GetObject(i), std::forward<Args>(args)...);
		}
		return returnvalues;
	}
	template< typename ReturnType>//, typename... Args>
	auto Call(ReturnType(*func)(T&))->std::array<decltype(func()),Size> {
		// Use std::invoke to call the member function with provided arguments
		std::array< ReturnType, Size> returnvalues;
		for (int i = 0; i < Size; i++)
		{
			returnvalues[i] = func(GetObject(i));
		}
		return returnvalues;
	}*/
	/*
	template< typename ReturnType, typename... Args>
	std::array< ReturnType, Size> Call(std::function<ReturnType(T&,Args...)> memberFunc, Args... args) {
		// Use std::invoke to call the member function with provided arguments
		std::array< ReturnType, Size> returnvalues;
		for (int i = 0; i < Size; i++)
		{
			returnvalues[i] = memberFunc(GetObject(i),std::forward<Args>(args)...);
		}
		return returnvalues;
	}

	template< typename ReturnType, typename... Args>
	std::array< ReturnType, Size> Call(std::function<ReturnType(const T&, Args...)> memberFunc, Args... args) const {
		// Use std::invoke to call the member function with provided arguments
		std::array< ReturnType, Size> returnvalues;
		for (int i = 0; i < Size; i++)
		{
			returnvalues[i] = memberFunc(GetObject(i),std::forward<Args>(args)...);
		}
		return returnvalues;
	}
	*/
	/*
	template< typename ReturnType>
	std::array< ReturnType, Size> Call(std::function<ReturnType(T&)> func) {
		// Use std::invoke to call the member function with provided arguments
		std::array< ReturnType, Size> returnvalues;
		for (int i = 0; i < Size; i++)
		{
			returnvalues[i] = func(GetObject(i));
		}
		return returnvalues;
	}*/
protected:
	constexpr T& GetObject(ObjectID id)
	{
		return reinterpret_cast<T&>( objects[id*sizeof(T)]);
	}
private:


	constexpr static std::deque<ObjectID> initialize_Queue()
	{
		std::deque<ObjectID> IDS(Size);
		std::iota(IDS.begin(), IDS.end(),0);
		return IDS;
	}

};
