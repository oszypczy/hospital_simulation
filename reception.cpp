#include "reception.h"

Reception::Reception(std::string id): Place(id){}

void Reception::addPatientToQueueFirst(std::unique_ptr<Patient> patient){
    RandomNumberGenerator generator;

    if (generator.percentage(50)){
        ushort serviceID = generator.chooseNumber(1, 65000);
        auto it = serviceDataBase.getServiceByID(serviceID);
        while (it != serviceDataBase.getMedicalServices().end()){
            serviceID = generator.chooseNumber(1, 65000);
            it = serviceDataBase.getServiceByID(serviceID);
        }
        bool NFZ = generator.chooseNumber(0, 1);
        ushort totalTime = 15;
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
        ushort totalTime = 60;
        OperationType type;
        Diseases disease = patient->getHealthCard().getDiseases()[0];
        if (disease == Diseases::HEART_ATTACK){
            type = OperationType::HEART_TRANSPLANT;
        } else {
            type = OperationType::BRAIN_TUMOR_REMOVAL;
        }
        serviceDataBase.addOperation(serviceID, totalTime, NFZ, type, disease);
        patient->getHealthCard().planService(serviceID);
    }
    patientsQueue.push_front(std::move(patient));
}

void Reception::addPatientToQueueLast(std::unique_ptr<Patient> patient){
    RandomNumberGenerator generator;

    if (generator.percentage(50)){
        ushort serviceID = generator.chooseNumber(1, 65000);
        auto it = serviceDataBase.getServiceByID(serviceID);
        while (it != serviceDataBase.getMedicalServices().end()){
            serviceID = generator.chooseNumber(1, 65000);
            it = serviceDataBase.getServiceByID(serviceID);
        }
        bool NFZ = generator.chooseNumber(0, 1);
        ushort totalTime = 15;
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
        ushort totalTime = 60;
        OperationType type;
        Diseases disease = patient->getHealthCard().getDiseases()[0];
        if (disease == Diseases::HEART_ATTACK){
            type = OperationType::HEART_TRANSPLANT;
        } else {
            type = OperationType::BRAIN_TUMOR_REMOVAL;
        }
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
    return tempPatient;
}

ServiceDataBase& Reception::getServiceDataBase(){
    return serviceDataBase;
}

void Reception::waitInReception(std::unique_ptr<Patient> patient){
    patientsQueue.push_front(std::move(patient));
}

std::deque<std::unique_ptr<Patient>>& Reception::getPatientsQueue(){
    return patientsQueue;
}
