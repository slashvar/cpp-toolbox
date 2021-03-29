#include <iostream>

#include "expected.hh"

Expected<int, std::string> divide(int a, int b)
{
    if (b == 0) {
        return { "division by 0" };
    }
    return { a / b };
}

int sqare_my_int(int x)
{
    return x * x;
}

int main()
{
    {
        auto res = divide(10, 2).and_then([](auto e) { return divide(e, 5); });
        if (res.has_value()) {
            std::cout << res.get() << std::endl;
        }
    }
    {
        auto res = divide(10, 0).and_then([](auto e) { return divide(e, 5); });
        if (res.has_value()) {
            std::cout << res.get() << std::endl;
        } else {
            std::cout << res.get_error() << std::endl;
        }
    }
    {
        auto res = divide(10, 2).map(sqare_my_int).map(sqare_my_int);
        if (res.has_value()) {
            std::cout << res.get() << std::endl;
        } else {
            std::cout << res.get_error() << std::endl;
        }
    }
    {
        auto res = divide(10, 0).map(sqare_my_int).map(sqare_my_int);
        if (res.has_value()) {
            std::cout << res.get() << std::endl;
        } else {
            std::cout << res.get_error() << std::endl;
        }
    }
}
