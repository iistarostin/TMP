#include <utility>
#include <type_traits>
#include <iostream>

template <long unsigned int index, typename ValueType>
class HierarchyLeaf
{
public:
    ValueType val;
};

template <typename IndexSequence, typename... Ty>
class HierarchyTargetBase;

template <long unsigned int... index, typename... Ty>
class HierarchyTargetBase <std::index_sequence<index...>, Ty...> : public HierarchyLeaf<index, Ty>...
{
public:
    HierarchyTargetBase()
        :HierarchyLeaf<index, Ty>()...
    {};
};

template <typename... Ty>
class HierarchyTarget : public HierarchyTargetBase<typename std::index_sequence_for<Ty...>, Ty...>
{
    using BaseType = HierarchyTargetBase<typename std::index_sequence_for<Ty...>, Ty...>;
public:
    HierarchyTarget()
        :BaseType()
    {};
};

template <typename>
constexpr long unsigned int indexByType(long unsigned int) { return -1; };

template <typename T, typename Head, typename... Tail>
constexpr long unsigned int indexByType(long unsigned int count = 0)
{
    return std::is_same<T, Head>::value ? count : indexByType<T, Tail...>(count+1);
};


template <long unsigned int, typename, typename...>
struct typeByIndex;

template <long unsigned int index, typename Head, typename... Tail>
struct typeByIndex
{
    typedef typename typeByIndex<index - 1, Tail...>::type type;
};

template <typename Head, typename... Tail>
struct typeByIndex<0, Head, Tail...>
{
    typedef Head type;
};


template<typename T, typename... Ty>
T& getByType(HierarchyTarget<Ty...>& h)
{
    return dynamic_cast<HierarchyLeaf<indexByType<T, Ty...>(), T>&>(h).val;
}

template<unsigned long int index, typename... Ty>
typename typeByIndex<index, Ty...>::type& getByIndex(HierarchyTarget<Ty...>& h)
{
    return dynamic_cast<HierarchyLeaf<index, typename typeByIndex<index, Ty...>::type>&>(h).val;
}

int main()
{
    HierarchyTarget<int, double> c;
    getByIndex<0, int, double>(c) = 5;
    getByType<double, int, double>(c) = 0.2;
    if (getByIndex<0, int, double>(c) == 5 &&
        getByType<double, int, double>(c) > 0)
        {
            std::cout << "Tests passed\n";
        }
        else 
        {
            std::cout << "Something went wrong\n";
        }
    return 0;
}