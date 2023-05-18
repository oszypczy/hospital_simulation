#pragma once
#include "place.h"
#include "patient.h"
#include "doctor.h"
#include "nurse.h"

#include <string>
#include <list>
#include <memory>


class Hospital : public Place{
    private:
        std::string name;
        std::list<std::unique_ptr<Place>> placesList;
        std::list<std::unique_ptr<Patient>> patientsList;
        std::list<std::unique_ptr<Nurse>> nursesList;
    public:
        Hospital(std::string name, std::list<std::unique_ptr<Place>> placesList, std::list<std::unique_ptr<Patient>> patientsList, std::list<std::unique_ptr<Nurse>> nursesList);
        void addPlace();
        void removePlace();
        void addPatient();
        void movePatient();
        void addNurse();
        void moveNurse();

};
