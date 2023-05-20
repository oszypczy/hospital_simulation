#pragma once

#include "room.h"
#include "patient.h"

#include <memory>
#include <list>

class GeneralRoom : public Room{
    private:
        ushort maxBeds;
        ushort attachedBeds;
        std::list<std::unique_ptr<Patient>> patientsList;
    public:
        GeneralRoom(ushort maxBeds);
        bool isFull() const;
        ushort checkOccupancy() const;
        void addPatient();
        void removePatient();

};
