#pragma once
#include "room.h"
#include "doctor.h"
#include "nurse.h"
#include "patient.h"
#include "consultation.h"
#include <memory>
#include <list>
#include <deque>
#include <utility>

class ConsultationRoom : public Room{
    private:
        std::deque<std::unique_ptr<Patient>> patientsQueue;
        int currentServiceID = -1;
        // std::list<std::unique_ptr<Doctor>> doctors;
        // std::list<std::unique_ptr<Nurse>> nurses;
        // std::unique_ptr<Patient> consultedPatient;
        // std::unique_ptr<Consultation> consultation;
    public:
        ConsultationRoom(std::string id);
        void addPatientToQueue(std::unique_ptr<Patient> patient);
        void removePatient(std::unique_ptr<Patient> patient);
        std::unique_ptr<Patient> getFirstPatientInQueue();
        int getCurrentServiceID() const;
        void setCurrentServiceID(int id);

        // void addDoctor(std::unique_ptr<Doctor> doctor);
        // void removeDoctor(std::unique_ptr<Doctor> doctor);
        // void addNurse(std::unique_ptr<Nurse> nurse);
        // void removeNurse(std::unique_ptr<Nurse> nurse);

        // std::unique_ptr<Patient> movePatient(std::unique_ptr<Patient> patient);
        // std::unique_ptr<Doctor> moveDoctor(std::unique_ptr<Doctor> doctor);
        // std::unique_ptr<Nurse> moveNurse(std::unique_ptr<Nurse> nurse);

        // void setConsultedPatient(std::unique_ptr<Patient> patient);
        // std::unique_ptr<Patient> moveConsultedPatient();

        // void setConsultation(std::unique_ptr<Consultation> consultation);

        // std::list<std::unique_ptr<Doctor>>& getDoctors();
        // std::list<std::unique_ptr<Nurse>>& getNurses();
        // std::unique_ptr<Patient>& getConsultedPatient();
        // std::unique_ptr<Consultation>& getConsultation();
};