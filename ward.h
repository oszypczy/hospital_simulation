#pragma once

#include "place.h"
#include "doctor.h"
#include "nurse.h"
#include "room.h"

#include <memory>
#include <list>

enum class WardType{
    psychiatric
};

class Ward : public Place{
    private:
        WardType type;
        std::list<std::unique_ptr<Nurse>> nurseList;
        std::list<std::unique_ptr<Doctor>> doctorList;
        std::list<std::unique_ptr<Room>> roomList;
    public:
        void addNurse();
        void addDoctor();
        void addRoom();

        void removeNurse();
        void removeDoctor();
        void removeRoom();

};
// lista sal, id - dziedziczy po miejscu, 
