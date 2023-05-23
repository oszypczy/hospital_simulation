#pragma once
#include "medicalService.h"
#include "doctor.h"
#include "healthCard.h"
#include <map>

enum class OperationType {HEART_TRANSPLANT, BRAIN_TUMOR_REMOVAL};

class Operation : public MedicalService {
    private:
        OperationType type;
        Diseases disease;
        const std::map<OperationType, DoctorSpecialty> requiredSecialty;
    public:
        Operation(ushort ID, unsigned short totalTime, bool NFZ, OperationType type, Diseases disease);
        ~Operation();
        bool checkPersonel() const;
        void startService(std::unique_ptr<Patient> patient);
        void continueService();
        void finishService();
        uint calculateCost() const;
        std::string print() const;
    friend std::ostream& operator<<(std::ostream& out, const Operation& Operation);
};