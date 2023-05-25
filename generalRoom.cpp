#include "generalRoom.h"
#include "invalidHumanPointer.h"

#include <algorithm>

GeneralRoom::GeneralRoom(std::string id, ushort maxBeds): Room(id), maxBeds(maxBeds){}


bool GeneralRoom::isFull() const{
    return attachedBeds == maxBeds;
}

ushort GeneralRoom::checkOccupancy() const{
    return attachedBeds;
}
void GeneralRoom::addPatient(std::unique_ptr<Patient> patient){
        patientsList.push_back(std::move(patient));
}
void GeneralRoom::removePatient(std::unique_ptr<Patient> patient){
    auto it = std::find_if(patientsList.begin(), patientsList.end(), [&](const std::unique_ptr<Patient>& p) {
        return *p == *patient;
    });

    if (it != patientsList.end()) {
        patientsList.erase(it);
    }
    else throw InvalidHumanPointer("Patient");
}

std::unique_ptr<Patient> GeneralRoom::movePatient(std::unique_ptr<Patient> patient){
    auto it = std::find_if(patientsList.begin(), patientsList.end(), [&](const std::unique_ptr<Patient>& p) {
        return *p == *patient;
    });

    if (it != patientsList.end()) {
        patientsList.erase(it);
    }
    else throw InvalidHumanPointer("Patient");

    return std::move(*it);
}

std::list<std::unique_ptr<Patient>>& GeneralRoom::getPatientsList(){
    return patientsList;
}
