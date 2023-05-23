#pragma once
#include <stdexcept>

class ZeroAgeException : public std::invalid_argument {
    public:
        ZeroAgeException();
};