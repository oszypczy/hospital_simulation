#include "reception.h"

Reception::Reception(std::string id): Place(id){}

void Reception::addPatientToQueueFirst(std::unique_ptr<Patient> patient){
    RandomNumberGenerator generator;

    if (generator.chooseNumber(0, 1) == 0){
        ushort serviceID = generator.chooseNumber(1, 65000);

        auto it = serviceDataBase.getServiceByID(serviceID);

        while (it != serviceDataBase.getMedicalServices().end()){
            serviceID = generator.chooseNumber(1, 65000);
            it = serviceDataBase.getServiceByID(serviceID);
        }
        bool NFZ = generator.chooseNumber(0, 1);
        ushort totalTime = generator.chooseNumber(1, 4) * 15;

        serviceDataBase.addConsultation(serviceID, totalTime, NFZ);
        patient->getHealthCard().planService(serviceID);
    } else {
        ushort serviceID = generator.chooseNumber(1, 65000);

        auto it = serviceDataBase.getServiceByID(serviceID);

        while (it != serviceDataBase.getMedicalServices().end()){
            serviceID = generator.chooseNumber(1, 65000);
            it = serviceDataBase.getServiceByID(serviceID);
        }
        bool NFZ = generator.chooseNumber(0, 1);
        ushort totalTime = generator.chooseNumber(1, 8) * 15;
        OperationType type = static_cast<OperationType>(generator.chooseNumber(0, 1));
        Diseases disease = static_cast<Diseases>(generator.chooseNumber(0, 2));

        serviceDataBase.addOperation(serviceID, totalTime, NFZ, type, disease);
        patient->getHealthCard().planService(serviceID);
    }
    patientsQueue.push_front(std::move(patient));
}

void Reception::addPatientToQueueLast(std::unique_ptr<Patient> patient){
    RandomNumberGenerator generator;

    if (generator.chooseNumber(0, 1) == 0){
        ushort serviceID = generator.chooseNumber(1, 65000);

        auto it = serviceDataBase.getServiceByID(serviceID);

        while (it != serviceDataBase.getMedicalServices().end()){
            serviceID = generator.chooseNumber(1, 65000);
            it = serviceDataBase.getServiceByID(serviceID);
        }
        bool NFZ = generator.chooseNumber(0, 1);
        ushort totalTime = generator.chooseNumber(1, 4) * 15;

        serviceDataBase.addConsultation(serviceID, totalTime, NFZ);
        patient->getHealthCard().planService(serviceID);
    } else {
        ushort serviceID = generator.chooseNumber(1, 65000);

        auto it = serviceDataBase.getServiceByID(serviceID);

        while (it != serviceDataBase.getMedicalServices().end()){
            serviceID = generator.chooseNumber(1, 65000);
            it = serviceDataBase.getServiceByID(serviceID);
        }
        bool NFZ = generator.chooseNumber(0, 1);
        ushort totalTime = generator.chooseNumber(1, 8) * 15;
        OperationType type = static_cast<OperationType>(generator.chooseNumber(0, 1));
        Diseases disease = static_cast<Diseases>(generator.chooseNumber(0, 2));

        serviceDataBase.addOperation(serviceID, totalTime, NFZ, type, disease);
        patient->getHealthCard().planService(serviceID);
    }
    patientsQueue.push_back(std::move(patient));
}

void Reception::addNurse(std::unique_ptr<Nurse> nurse){
    this->nurse = std::move(nurse);
}

std::unique_ptr<Nurse> Reception::moveNurse(){
    return std::move(nurse);
}

std::unique_ptr<Patient> Reception::movePatient(){
    auto tempPatient = std::move(patientsQueue.front());
    patientsQueue.pop_front();
    return std::move(tempPatient);
}

ServiceDataBase& Reception::getServiceDataBase(){
    return serviceDataBase;
}
