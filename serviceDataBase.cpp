#include "serviceDataBase.h"
#include "consultation.h"
#include "operation.h"
#include "objectNotFoundException.h"
#include "objectAlreadyExistsException.h"
#include <iostream>
#include <algorithm>

void ServiceDataBase::addConsultation(ushort ID, unsigned short totalTime, bool NFZ){
    /*
    * Adds a consultation to the service database.
    * @param ID The ID of the consultation.
    * @param totalTime The total time of the consultation.
    * @param NFZ A boolean indicating whether the consultation is covered by NFZ (National Health Fund).
    * @throws ObjectAlreadyExistsException if a consultation with the given ID already exists in the database.
    */
    std::unique_ptr<Consultation> addedConcultation = std::make_unique<Consultation>(ID, totalTime, NFZ);
    if (getServiceByID(ID) == medicalServices.end()){
        medicalServices.push_back(std::move(addedConcultation));
    } else {
        throw ObjectAlreadyExistsException("Consultation with given ID");
    }
}

void ServiceDataBase::addOperation(ushort ID, unsigned short totalTime, bool NFZ, OperationType type, Diseases disease){
    /*
    * Adds an operation to the service database.
    * @param ID The ID of the operation.
    * @param totalTime The total time of the operation.
    * @param NFZ A boolean indicating whether the operation is covered by NFZ (National Health Fund).
    * @param type The type of operation.
    * @param disease The disease associated with the operation.
    * @throws ObjectAlreadyExistsException if an operation with the given ID already exists in the database.
    */
    std::unique_ptr<Operation> addedOperation = std::make_unique<Operation>(ID, totalTime, NFZ, type, disease);
    if (getServiceByID(ID) == medicalServices.end()){
        medicalServices.push_back(std::move(addedOperation));
    } else {
        throw ObjectAlreadyExistsException("Operation with given ID");
    }
}

std::list<std::unique_ptr<MedicalService>>::iterator ServiceDataBase::getServiceByID(ushort givenID){
    /*
    * Retrieves a service iterator from the service database based on the given ID.
    * @param givenID The ID of the service to retrieve.
    * @return An iterator pointing to the service if found, or medicalServices.end() if not found.
    */
    auto it = std::find_if(medicalServices.begin(), medicalServices.end(), [givenID](const std::unique_ptr<MedicalService>& service) {
        return service->getID() == givenID;
    });
    return it;
}

void ServiceDataBase::removeService(ushort givenID){
    /*
    * Removes a service from the service database based on the given ID.
    * @param givenID The ID of the service to remove.
    * @throws ObjectNotFoundException if a service with the given ID is not found in the database.
    */
    auto it = getServiceByID(givenID);
    if (it != medicalServices.end()){
        medicalServices.erase(it);
    } else {
        throw ObjectNotFoundException("Service with given ID");
    }
}

uint ServiceDataBase::calculateTotalCost() const{
    /*
    * Calculates the total cost of all services in the database.
    * @return The total cost of all services.
    */
    uint totalCost = 0;
    for(auto& service : medicalServices){
        totalCost += service->calculateCost();
    }
    return totalCost;
}

std::list<std::unique_ptr<MedicalService>>& ServiceDataBase::getMedicalServices(){
    /*
    * Retrieves the medical services from the database.
    * @return The medical services from the database.
    */
    return medicalServices;
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
