#pragma once
#include <stdexcept>
#include "string"

class WrongServiceStateException : public std::logic_error {
    public:
        WrongServiceStateException(std::string invalidState, std::string expectedState);
};