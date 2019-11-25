#pragma once

#include <tuple>
#include <utility>

template <typename Callable, typename... Args>
class BindingFunctor
{
    const Callable& target;
    std::tuple<Args...> boundArgs;
public:
    BindingFunctor(const Callable& callable, Args&&... args)
        :target(callable), boundArgs(std::forward<Args>(args)...)
        {}

    template <typename Owner, std::size_t... Ind>
    auto invokeSubst(Owner&& owner, std::index_sequence<Ind...>) -> decltype((owner.*target)(std::get<Ind>(boundArgs)...))
    {
        return (owner.*target)(std::get<Ind>(boundArgs)...);
    };

    template <std::size_t... Ind>
    auto invokeSubst(std::index_sequence<Ind...>) -> decltype(target(std::get<Ind>(boundArgs)...))
    {
        return target(std::get<Ind>(boundArgs)...);
    };

    template <typename Owner, typename argsIndexSequence = std::make_index_sequence<sizeof...(Args)>>
    auto invoke(Owner&& owner) -> decltype(invokeSubst(std::forward<Owner>(owner), argsIndexSequence{}))
    {
        return invokeSubst(std::forward<Owner>(owner), argsIndexSequence{});
    };

    template <typename argsIndexSequence = std::make_index_sequence<sizeof...(Args)>>
    auto invoke() -> decltype(invokeSubst(argsIndexSequence{}))
    {
        return invokeSubst(argsIndexSequence{});
    };
};

template<class Callable, typename...Args>
BindingFunctor<Callable, Args...> makeBindingFunctor(const Callable& callable, Args&&... args) 
{
    return BindingFunctor<Callable, Args...>(callable, std::forward<Args>(args)...);
}