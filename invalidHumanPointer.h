#pragma once

#include <stdexcept>

class InvalidHumanPointer : public std::invalid_argument{
    public:
        InvalidHumanPointer(std::string message);
};