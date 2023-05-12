#pragma once
#include <stdexcept>

class InvalidRegistrationNumberException : public std::invalid_argument {
    public:
        InvalidRegistrationNumberException(std::string invalidNumber);
};