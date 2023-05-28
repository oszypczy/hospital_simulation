#pragma once
#include "medicalService.h"
#include "operation.h"
#include <list>
#include <memory>

class ServiceDataBase {
    private:
        std::list<std::unique_ptr<MedicalService>> medicalServices;
    public:
        uint calculateTotalCost() const;
        std::list<std::unique_ptr<MedicalService>>::iterator getServiceByID(ushort ID);
        void addConsultation(ushort ID, unsigned short totalTime, bool NFZ);
        void addOperation(ushort ID, unsigned short totalTime, bool NFZ, OperationType type, Diseases disease);
        void removeService(ushort ID);

        std::list<std::unique_ptr<MedicalService>>& getMedicalServices();
    friend std::ostream& operator<<(std::ostream& os, const ServiceDataBase& database);
};