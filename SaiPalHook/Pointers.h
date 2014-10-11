#pragma once
#include <iostream>
#include <string>

void* GetModuleBase();
void* GetModuleBase(const std::string& ModuleName);
inline void* GetBasePointer()
{
	return GetModuleBase();
}

class Pointer
{
public:
	Pointer() : _Pointer(nullptr)
	{
	}
	Pointer(void* Pointer) : _Pointer(Pointer)
	{
	}
	Pointer(unsigned int Offset) : _Pointer((void*)Offset)
	{
	}
	~Pointer()
	{
	}

	//Resolves offsets the current pointer by the offset and picks up the 4 byte integer value
	// at the new location and returns it as a new pointer object
	Pointer operator [](const unsigned int Offset) const
	{
		if (_Pointer)
		{
			if ((unsigned int*)((char*)_Pointer + Offset))
			{
				//std::cout << std::hex;
				//std::cout << "Resolving: (" << _Pointer << " + " << Offset << ") " << (unsigned int*)((char*)_Pointer + Offset) << std::endl;
				//std::cout << "Value is : " << *(unsigned int*)((char*)_Pointer + Offset) << std::endl;
				return Pointer(*(unsigned int*)((char*)_Pointer + Offset));
			}
		}
		return Pointer(nullptr);
	}

	template <typename T>
	T* operator ->() const
	{
		return (T*)_Pointer;
	}

	template <typename T>
	T as() const
	{
		if (_Pointer)
		{
			return *(T*)_Pointer;
		}
		return T();
	}

	//Returns the position of the pointer offset by a number of bytes
	Pointer operator() (unsigned int Offset = 0, unsigned int Stride = 1)
	{
		return Pointer((unsigned char*)_Pointer + (Offset*Stride));
	}

	//Assignment
	template <class T>
	void operator= (T* Pointer)
	{
		_Pointer = (void*)Pointer;
	}

	void operator= (Pointer Pointer)
	{
		_Pointer = Pointer._Pointer;
	}

	//Implicit cast to any other pointer type
	template <class T>
	operator T* () const
	{
		return (T*)_Pointer;
	}

	operator bool() const
	{
		return _Pointer != nullptr ? true : false;
	}

	unsigned int asUint()
	{
		return as<unsigned int>();
	}

	unsigned short asUShort()
	{
		return as<unsigned short>();
	}

	unsigned char asUchar()
	{
		return as<unsigned char>();
	}

	void Set(unsigned int Pointer)
	{
		_Pointer = (void*)Pointer;
	}

private:
	void* _Pointer;
};