#pragma once 

#include "FunctionTraits.h"
#include "FunctionWrapper.h"
#include "BindingFunctor.h"

class TestClass
{   
public:
    int f(float a, double b)
    {
        return (int)(a * b);
    }
};

long f(float a, double b)
{
    return (int)(a / b);
}

void testTraits() 
{
    static_assert(std::is_same<FunctionTraits<decltype(&f)>::ReturnType, long>::value, "");
    static_assert(std::is_same<FunctionTraits<decltype(&f)>::ArgumentAt<0>::type, float>::value, "");
    static_assert(std::is_same<FunctionTraits<decltype(&f)>::ArgumentAt<1>::type, double>::value, "");
    static_assert(std::is_same<FunctionTraits<decltype(&f)>::OwnerType, void>::value, "");

    static_assert(std::is_same<FunctionTraits<decltype(&TestClass::f)>::ReturnType, int>::value, "");
    static_assert(std::is_same<FunctionTraits<decltype(&TestClass::f)>::ArgumentAt<0>::type, TestClass&>::value, "");
    static_assert(std::is_same<FunctionTraits<decltype(&TestClass::f)>::ArgumentAt<1>::type, float>::value, "");
    static_assert(std::is_same<FunctionTraits<decltype(&TestClass::f)>::OwnerType, TestClass>::value, "");
}

bool testFunctionWrapper() 
{
    auto ftr = makeFunctionWrapper(f);
    auto ftr2 = makeFunctionWrapper(&TestClass::f);
    auto ftr3 = makeFunctionWrapper((int(*)(int))([](int a) {return a+1;}));
    TestClass myObj;
    return ftr.invoke(4, 3) == 1 && ftr2.invoke(myObj, 6, 4) == 24 && ftr3.invoke(5) == 6; 
}

bool testBindgingFunctor()
{
    auto ftr = makeBindingFunctor(f, 4, 3);
    auto ftr2 = makeBindingFunctor(&TestClass::f, 6, 4);
    auto ftr3 = makeBindingFunctor((int(*)(int))([](int a) {return a+1;}), 5);
    TestClass myObj;
    return ftr.invoke() == 1 && ftr2.invoke(myObj) == 24 && ftr3.invoke() == 6; 
}

bool runAllTests() 
{
    testTraits();
    if (!testFunctionWrapper())
    {
        std::cout << "Function wrapper tests failed\n";
        return 1;
    }
    if (!testBindgingFunctor())
    {
        std::cout << "Binding functor tests failed\n";
        return 1;
    }
    return 0;
}