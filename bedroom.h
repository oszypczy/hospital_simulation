#pragma once

#include "room.h"

#include <memory>
#include <list>

class Bedroom : public Room{
    private:
        const uint8_t maxBeds;
        uint8_t freeBeds;
        std::list<std::unique_ptr<Patient>> patients;
    public:
        void addPatient();
        void removePatient();
        std::list<std::unique_ptr<Patient>> getPatients() const;
};