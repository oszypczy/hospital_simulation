#pragma once

#include "place.h"
#include "doctor.h"
#include "nurse.h"
#include "room.h"

#include <memory>
#include <list>
#include <string>



class Ward : public Place{
    private:
        std::string name;
        std::list<std::unique_ptr<Room>> roomList;
    public:
        Ward(std::string id, std::string name, std::list<std::unique_ptr<Room>> roomList);
        std::string getName() const;
};
