#pragma once
#include <tuple>

template<typename F>
struct FunctionTraits;
 
template<typename Ret, typename... Args>
struct FunctionTraits<Ret(*)(Args...)>
{
    using ReturnType = Ret;
    using OwnerType = void;
    static constexpr std::size_t argumentsCount = sizeof...(Args);
    using arguments = std::tuple<Args...>;

    template <std::size_t index>
    struct ArgumentAt
    {
        using type = typename std::tuple_element<index,arguments>::type;
    };
};

template<typename ThisType, class Ret, class... Args>
struct FunctionTraits<Ret(ThisType::*)(Args...)> : public FunctionTraits<Ret(*)(ThisType&,Args...)>
{
    using OwnerType = ThisType;
};