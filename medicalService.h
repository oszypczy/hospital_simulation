#pragma once
#include <vector>
#include <list>
#include <memory>
#include <string>
#include "doctor.h"
#include "nurse.h"
#include "patient.h"

enum class ServiceState {NOT_READY, IN_PROGRESS, FINISHED};

class MedicalService {
    protected:
        unsigned short totalTime;
        unsigned short progressTime = 0;
        std::list<std::unique_ptr<Doctor>> doctors;
        std::list<std::unique_ptr<Nurse>> nurses;
        std::unique_ptr<Patient> patient;
        ServiceState state = ServiceState::NOT_READY;
        bool NFZ;
        ushort ID;
    public:
        MedicalService(ushort ID, unsigned short totalTime, bool NFZ);
        virtual ~MedicalService();
        void operator++();
        ServiceState getServiceState() const;
        void changeState(ServiceState state);
        unsigned short getProgressTime() const;
        void addDoctor(std::unique_ptr<Doctor> doctor);
        void addNurse(std::unique_ptr<Nurse> nurse);
        Patient& getPatient() const;
        Doctor& getDoctor(std::string PESEL) const;
        Nurse& getNurse(std::string PESEL) const;
        ushort getID() const;
        Doctor& getDoctor(DoctorSpecialty specialty) const;
        virtual uint calculateCost() const = 0;
        virtual std::string print() const = 0;
        virtual bool checkPersonel() const = 0;
        virtual void startService(std::unique_ptr<Patient> patient) = 0;
        virtual void continueService() = 0;
        virtual void finishService() = 0;
    friend std::ostream& operator<<(std::ostream& os, const MedicalService& service);
};
