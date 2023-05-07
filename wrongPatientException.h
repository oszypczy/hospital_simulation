#pragma once
#include <stdexcept>

class WrongPatientException : public std::logic_error {
    public:
        WrongPatientException();
};