#include <cstdlib>
#include <ctime>
#include <iostream>
#include "identify.hpp"

int main(void)
{
    std::srand(std::time(0));   // seed once at program start

    Base* ptr = generate();

    std::cout << "Identify by pointer: ";
    identify(ptr);

    std::cout << "Identify by reference: ";
    identify(*ptr);

    delete ptr;
    return 0;
}