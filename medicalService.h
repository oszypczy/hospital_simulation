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
        std::unique_ptr<Doctor> addDoctor(std::unique_ptr<Doctor> doctor);
        std::unique_ptr<Nurse> addNurse(std::unique_ptr<Nurse> nurse);
        Patient& getPatient() const;
        std::list<std::unique_ptr<Doctor>>::iterator getDoctor(std::string PESEL);
        std::list<std::unique_ptr<Nurse>>::iterator getNurse(std::string PESEL);
        ushort getID() const;
        Doctor& getDoctor(DoctorSpecialty specialty) const;
        Doctor& getFirstDoctor() const;
        Nurse& getFirstNurse() const;
        virtual uint calculateCost() const = 0;
        virtual std::string print() const = 0;
        virtual bool checkPersonel() const = 0;
        virtual void startService(std::unique_ptr<Patient> patient) = 0;
        virtual void continueService() = 0;
        virtual void finishService() = 0;
        std::unique_ptr<Patient> returnPatient();
        std::unique_ptr<Doctor> returnDoctor(std::string PESEL);
        std::unique_ptr<Nurse> returnNurse(std::string PESEL);
    friend std::ostream& operator<<(std::ostream& os, const MedicalService& service);
};
