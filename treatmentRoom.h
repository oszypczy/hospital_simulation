#pragma once

#include "room.h"
#include "doctor.h"
#include "nurse.h"
#include "patient.h"
#include "operation.h"
#include "operation.h"

#include <memory>
#include <list>
#include  <deque>

class TreatmentRoom : public Room{
    private:
        std::list<std::unique_ptr<Doctor>> doctors;
        std::list<std::unique_ptr<Nurse>> nurses;
        std::deque<Patient> patientsQueue;
        Patient treatedPatient;
        Operation operation;
    public:
        TreatmentRoom(std::string id);
        void proceedTreatment();
        void addPatientToQueue(std::unique_ptr<Patient> patient);
        void removePatient(std::unique_ptr<Patient> patient);
        void addDoctor();
        void removeDoctor();
        void addNurse();
        void removeNurse();
};