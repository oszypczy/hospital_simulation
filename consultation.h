#pragma once
#include "medicalService.h"

class Consultation : public MedicalService {
    public:
        Consultation(ushort ID, unsigned short totalTime, bool NFZ);
        ~Consultation();
        bool checkPersonel() const;
        void startService(std::unique_ptr<Patient> patient);
        void continueService();
        void finishService();
        uint calculateCost() const;
        std::string print() const;
    friend std::ostream& operator<<(std::ostream& out, const Consultation& consultation);
};