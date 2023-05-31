#include "treatmentRoom.h"
#include "invalidHumanPointer.h"

#include <algorithm>

TreatmentRoom::TreatmentRoom(std::string id): Room(id){}


void TreatmentRoom::addPatientToQueue(std::unique_ptr<Patient> patient){
    patientsQueue.push_back(std::move(patient));
}

std::unique_ptr<Patient> TreatmentRoom::getFirstPatientInQueue(){
    auto it = std::move(patientsQueue.front());
    patientsQueue.pop_front();
    return it;
}

int TreatmentRoom::getCurrentServiceID() const{
    return currentServiceID;
}

void TreatmentRoom::setCurrentServiceID(int id){
    currentServiceID = id;
}

bool TreatmentRoom::checkIfAnyoneInQueue(){
    return !patientsQueue.empty();
}

Diseases TreatmentRoom::getDiseaseOfFirstPatientInQueue() const{
    return patientsQueue.front()->getHealthCard().getDiseases()[0];
}
