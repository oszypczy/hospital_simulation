#pragma once

#include <stdexcept>

class InvalidPlaceIDs : public std::logic_error{
    public:
        InvalidPlaceIDs();
};