#pragma once
#include <stdexcept>

class InvalidPeselException: public std::invalid_argument {
    public:
        InvalidPeselException(std::string invalidPESEL);
};