#pragma once
#include <stdexcept>
#include "string"

class ObjectNotFoundException : public std::logic_error {
    public:
        ObjectNotFoundException(std::string message);
};