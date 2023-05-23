#pragma once
#include <stdexcept>
#include "string"

class EmptyAttributeException : public std::invalid_argument {
    public:
        EmptyAttributeException(std::string attributeType);
};
