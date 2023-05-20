#pragma once

#include "place.h"
#include "doctor.h"
#include "nurse.h"
#include "room.h"

#include <memory>
#include <list>
#include <string>

enum class WardType{
    psychiatric
};

class Ward : public Place{
    private:
        std::string name;
        std::list<std::unique_ptr<Room>> roomList;
    public:


};
