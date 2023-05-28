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
        std::list<std::string> patientsPESELList;
    public:
        GeneralRoom(std::string id, ushort maxBeds);
        bool isFull() const;
        ushort checkOccupancy() const;
        void addPatient(std::unique_ptr<Patient> patient);
        void removePatient(std::unique_ptr<Patient> patient);

        std::unique_ptr<Patient> movePatient(std::unique_ptr<Patient> patient);

        std::list<std::unique_ptr<Patient>>& getPatientsList();

        std::list<std::string>& getPatientsPESELList();
};
