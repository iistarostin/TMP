#include <iostream>

struct NullType{};

template <typename H, typename T>
struct TypeList
{
	typedef H Head;
	typedef T Tail;
};

template <typename, typename>
struct  getIndexOf;

template <typename Key>
struct getIndexOf<Key, NullType> 
{
	static const int value = -1;
};

template <typename Key, typename T>
struct getIndexOf<Key, TypeList<Key, T>>
{
	static const int value = 0;
};

template <typename Key, typename H, typename T>
struct getIndexOf<Key, TypeList<H, T>>
{
	static const int res = getIndexOf<Key, T>::value;
	static const int value = res == -1 ? -1 : res + 1;
};


void main()
{
	//I guess modern C++ is allowed for testing purposes
	using MyList = TypeList<int, TypeList<double, TypeList<long, NullType>>>;
	static_assert(getIndexOf<double, MyList>::value == 1, "Test failed");
	static_assert(getIndexOf<std::string, MyList>::value == -1, "Test failed");
	static_assert(getIndexOf<long, MyList>::value == 2, "Test failed");
	static_assert(getIndexOf<int, MyList>::value == 0, "Test failed");
}