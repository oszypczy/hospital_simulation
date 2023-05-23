#pragma once
#include <stdexcept>

class WrongPersonStateException : public std::logic_error {
    public:
        WrongPersonStateException(std::string invalidState, std::string expectedState);
};