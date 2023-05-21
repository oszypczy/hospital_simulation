#pragma once

#include <stdexcept>
#include <string>

class EmptyIDException : public std::invalid_argument{
    public:
        EmptyIDException();
};