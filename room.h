#pragma once

#include "place.h"

#include <string>

class Room : public Place{
    public:
        Room(std::string id);
        ~Room();
};
