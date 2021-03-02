#include <string>
#include "common.hpp"

int main()
{
    int a = 1;
    float b = 3.14;
    std::string str = "Var is str";
    Logger::debug("this a=", a, str, "b is ", b);
    Logger::warn("this is a warning:\ta=", a, str, "b is ", b);
}