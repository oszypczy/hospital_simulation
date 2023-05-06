#pragma once
#include <stdexcept>

class WrongPersonelException : public std::logic_error {
    public:
        WrongPersonelException();
};