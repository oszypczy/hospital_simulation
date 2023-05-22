#pragma once

#include "place.h"

#include <string>

class Room : public Place{
    private:
        ushort number;
    public:
        Room(std::string id, ushort number);
        ~Room();
};
