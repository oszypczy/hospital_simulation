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
        // std::list<std::unique_ptr<Doctor>> doctors;
        // std::list<std::unique_ptr<Nurse>> nurses;
        std::deque<std::unique_ptr<Patient>> patientsQueue;
        int currentServiceID = -1;
        // std::unique_ptr<Patient> treatedPatient;
        // std::unique_ptr<Operation> operation;
    public:
        TreatmentRoom(std::string id);
        void addPatientToQueue(std::unique_ptr<Patient> patient);
        void removePatient(std::unique_ptr<Patient> patient);
        std::unique_ptr<Patient> getFirstPatientInQueue();
        int getCurrentServiceID() const;
        void setCurrentServiceID(int id);
        bool checkIfAnyoneInQueue();
        Diseases getDiseaseOfFirstPatientInQueue() const;
        // void addDoctor(std::unique_ptr<Doctor> doctor);
        // void removeDoctor(std::unique_ptr<Doctor> doctor);
        // void addNurse(std::unique_ptr<Nurse> nurse);
        // void removeNurse(std::unique_ptr<Nurse> nurse);

        // std::unique_ptr<Patient> movePatient(std::unique_ptr<Patient> patient);
        // std::unique_ptr<Doctor> moveDoctor(std::unique_ptr<Doctor> doctor);
        // std::unique_ptr<Nurse> moveNurse(std::unique_ptr<Nurse> nurse);

        // void setTreatedPatient(std::unique_ptr<Patient> patient);
        // std::unique_ptr<Patient> moveTreatedPatient();

        // void setOperation(std::unique_ptr<Operation> operation);

        // std::list<std::unique_ptr<Doctor>>& getDoctors();
        // std::list<std::unique_ptr<Nurse>>& getNurses();
        // std::unique_ptr<Patient>& getTreatedPatient();
        // std::unique_ptr<Operation>& getOperation();
};
