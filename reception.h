#pragma once

#include "place.h"
#include "nurse.h"
#include "patient.h"
#include "randomNumberGenerator.h"
#include "serviceDataBase.h"

#include <memory>
#include <deque>

class Reception : public Place{
    private:
        std::unique_ptr<Nurse> nurse;
        std::deque<std::unique_ptr<Patient>> patientsQueue;
        ServiceDataBase serviceDataBase;
        std::unique_ptr<Patient> generateService(std::unique_ptr<Patient> patient);
    public:
        Reception(std::string id);
        void addPatientToQueueFirst(std::unique_ptr<Patient> patient);
        void waitInReception(std::unique_ptr<Patient> patient);
        void addPatientToQueueLast(std::unique_ptr<Patient> patient);
        void addNurse(std::unique_ptr<Nurse> nurse);
        std::unique_ptr<Nurse> moveNurse();
        std::unique_ptr<Patient> movePatient();
        std::deque<std::unique_ptr<Patient>>& getPatientsQueue();
        ServiceDataBase& getServiceDataBase();
};
