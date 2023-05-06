#pragma once
#include <stdexcept>
#include "string"

class ObjectAlreadyExistsException : public std::logic_error {
    public:
        ObjectAlreadyExistsException(std::string message);
};