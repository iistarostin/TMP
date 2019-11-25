#pragma once

template<typename F>
class FunctionWrapper;

template<typename Ret, typename... Args>
class FunctionWrapper<Ret(*)(Args...)>
{
protected:
    using F = Ret(*)(Args...);
    F f;
public:
    FunctionWrapper(F func)
        :f(func) {}
    
    Ret invoke(Args&&... args) 
    {
        return f(std::forward<Args>(args)...);
    }
};

template <typename Owner, typename Ret, typename... Args>
class FunctionWrapper<Ret(Owner::*)(Args...)>
{
protected:
    using F = Ret(Owner::*)(Args...);
    F f;
public:
    FunctionWrapper(F func)
        :f(func) {}
    
    Ret invoke(Owner& owner, Args&&... args) 
    {
        return (owner.*f)(std::forward<Args>(args)...);
    }

    Ret invoke(const Owner& owner, Args&&... args) 
    {
        return (owner.*f)(std::forward<Args>(args)...);
    }

    Ret invoke(Owner&& owner, Args&&... args) 
    {
        return (owner.*f)(std::forward<Args>(args)...);
    }
};

template<typename F>
auto makeFunctionWrapper(F f)
{
    return FunctionWrapper<F>(f);
};