#include "consultationRoom.h"
#include "invalidHumanPointer.h"

#include <algorithm>

ConsultationRoom::ConsultationRoom(std::string id): Room(id){}


void ConsultationRoom::addPatientToQueue(std::unique_ptr<Patient> patient){
    patientsQueue.push_back(std::move(patient));
}


void ConsultationRoom::removePatient(std::unique_ptr<Patient> patient){
    auto it = std::find_if(patientsQueue.begin(), patientsQueue.end(), [&](const std::unique_ptr<Patient>& p) {
        return *p == *patient;
    });

    if (it != patientsQueue.end()) {
        patientsQueue.erase(it);
    }
    else throw InvalidHumanPointer("Patient");
}

std::unique_ptr<Patient> ConsultationRoom::getFirstPatientInQueue(){
    auto it = std::move(patientsQueue.front());
    patientsQueue.pop_front();
    return it;
}

int ConsultationRoom::getCurrentServiceID() const{
    return currentServiceID;
}

void ConsultationRoom::setCurrentServiceID(int id){
    currentServiceID = id;
}

bool ConsultationRoom::checkIfAnyoneInQueue(){
    return !patientsQueue.empty();
}
