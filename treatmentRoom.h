#pragma once

#include "room.h"
#include "doctor.h"
#include "nurse.h"
#include "patient.h"
#include "operation.h"
#include "operation.h"

#include <memory>
#include <list>
#include <deque>
#include <utility>

class TreatmentRoom : public Room{
    private:
        std::deque<std::unique_ptr<Patient>> patientsQueue;
        int currentServiceID = -1;
    public:
        TreatmentRoom(std::string id);
        void addPatientToQueue(std::unique_ptr<Patient> patient);
        std::unique_ptr<Patient> getFirstPatientInQueue();
        int getCurrentServiceID() const;
        void setCurrentServiceID(int id);
        bool checkIfAnyoneInQueue();
        Diseases getDiseaseOfFirstPatientInQueue() const;
};
