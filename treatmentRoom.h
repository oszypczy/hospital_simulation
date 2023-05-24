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
        std::list<std::unique_ptr<Doctor>> doctors;
        std::list<std::unique_ptr<Nurse>> nurses;
        std::deque<std::unique_ptr<Patient>> patientsQueue;
        std::unique_ptr<Patient> treatedPatient;
        std::unique_ptr<Operation> operation;
    public:
        TreatmentRoom(std::string id);
        void addPatientToQueue(std::unique_ptr<Patient> patient);
        void removePatient(std::unique_ptr<Patient> patient);
        void addDoctor(std::unique_ptr<Doctor>);
        void removeDoctor(std::unique_ptr<Doctor>);
        void addNurse(std::unique_ptr<Nurse>);
        void removeNurse(std::unique_ptr<Nurse>);

        std::unique_ptr<Patient> movePatient();
        std::unique_ptr<Doctor> moveDoctor();
        std::unique_ptr<Nurse> moveNurse();

};