#include "serviceDataBase.h"
#include "consultation.h"
#include "operation.h"
#include "objectNotFoundException.h"
#include "objectAlreadyExistsException.h"
#include <iostream>
#include <algorithm>

void ServiceDataBase::addConsultation(ushort ID, unsigned short totalTime, bool NFZ){
    std::unique_ptr<Consultation> addedConcultation = std::make_unique<Consultation>(ID, totalTime, NFZ);
    if (getServiceByID(ID) == medicalServices.end()){
        medicalServices.push_back(std::move(addedConcultation));
    } else {
        throw ObjectAlreadyExistsException("Consultation with given ID");
    }
}

void ServiceDataBase::addOperation(ushort ID, unsigned short totalTime, bool NFZ, OperationType type){
    std::unique_ptr<Operation> addedOperation = std::make_unique<Operation>(ID, totalTime, NFZ, type);
    if (getServiceByID(ID) == medicalServices.end()){
        medicalServices.push_back(std::move(addedOperation));
    } else {
        throw ObjectAlreadyExistsException("Operation with given ID");
    }
}

std::list<std::unique_ptr<MedicalService>>::iterator ServiceDataBase::getServiceByID(ushort givenID){
    auto it = std::find_if(medicalServices.begin(), medicalServices.end(), [givenID](const std::unique_ptr<MedicalService>& service) {
        return service->getID() == givenID;
    });
    return it;
}

void ServiceDataBase::removeService(ushort givenID){
    auto it = getServiceByID(givenID);
    if (it != medicalServices.end()){
        medicalServices.erase(it);
    } else {
        throw ObjectNotFoundException("Service with given ID");
    }
}

uint ServiceDataBase::calculateTotalCost() const{
    uint totalCost = 0;
    for(auto& service : medicalServices){
        totalCost += service->calculateCost();
    }
    return totalCost;
}

std::ostream& operator<<(std::ostream& os, const ServiceDataBase& database){
    if (database.medicalServices.empty()){
        os << "No services in database" << std::endl;
        return os;
    }
    for(const auto& service : database.medicalServices){
        os << "--------------------------------------------" << std::endl;
        os << (*service) << std::endl;
    }
    return os;
}
