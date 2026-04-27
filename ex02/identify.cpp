#include <cstdlib>
#include <exception>
#include <iostream>
#include "identify.hpp"

Base* generate(void)
{
    int value = std::rand() % 3;

    if (value == 0)
        return new A();
    if (value == 1)
        return new B();
    return new C();
}

void identify(Base* p)
{
    if (dynamic_cast<A*>(p))
        return (void)(std::cout << "A" << std::endl);
    if (dynamic_cast<B*>(p))
        return (void)(std::cout << "B" << std::endl);
    if (dynamic_cast<C*>(p))
        return (void)(std::cout << "C" << std::endl);
}

void identify(Base& p)
{
    try
    {
        (void)dynamic_cast<A&>(p);
        std::cout << "A" << std::endl;
        return;
    }
    catch (const std::exception&) {}

    try
    {
        (void)dynamic_cast<B&>(p);
        std::cout << "B" << std::endl;
        return;
    }
    catch (const std::exception&) {}

    try
    {
        (void)dynamic_cast<C&>(p);
        std::cout << "C" << std::endl;
        return;
    }
    catch (const std::exception&) {}
}