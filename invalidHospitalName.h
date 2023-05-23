#pragma once

#include <stdexcept>

class InvalidHospitalName : public std::invalid_argument{
    public:
        InvalidHospitalName();
};
